% Failing moves become holds
hold(PR, PL) :- province(PR), player(PL), unsuccess(convoy(D, PR, F, PL)), convoy(D, PR, F, PL).
hold(PR, PL) :- province(PR), player(PL), unsuccess(move(D, PR, PL)), move(D, PR, PL).

% Counts the number of Xs that satisfy Y, stores in N
count(X, Y, N) :- findall(X, Y, L), length(L, N). 

% Gives a value in a list
contains([X], X).
contains([X|_], X).
contains([_|T], X) :- contains(T, X).

% True if list of provinces
provList([]).
provList([X]) :- province(X).
provList([X|T]) :- province(X), provList(T).

% Verifies its a valid power value
validpower(N) :- count(X, province(X), M), integer(N), between(0, M, N).

% Convoy cuts if anything moves into fleet location
cut(convoy(D, S, F, PL)) :- convoy(D, S, F, PL), provList(F), province(X), contains(F, X), move(X, S, PL2), province(S), player(PL2).

% Support cuts if anything is not blocked and tries to move into the province
cut(supporthold(PR, PL, hold(PR2, PL2))) :-
    hold(PR2, PL2),
    supporthold(PR, PL, hold(PR2, PL2)),
    province(S),
    player(PL3),
    move(PR, S, PL3).
cut(supporthold(PR, PL, hold(PR2, PL2))) :-
    hold(PR2, PL2),
    supporthold(PR, PL, hold(PR2, PL2)),
    province(S),
    player(PL3),
    convoy(PR, S, F, PL3),
    not(cut(convoy(PR, S, F, PL3))).

cut(supportconvoy(PR, PL, convoy(D, S, F, PL2))) :-
    convoy(D, S, F, PL2),
    supportconvoy(PR, PL, convoy(D, S, F, PL2)),
    province(S2),
    player(PL3),
    move(PR, S2, PL3).
cut(supportconvoy(PR, PL, convoy(D, S, F, PL2))) :-
    convoy(D, S, F, PL2),
    supportconvoy(PR, PL, convoy(D, S, F, PL2)),
    province(S2),
    player(PL3),
    convoy(PR, S2, F2, PL3),
    not(cut(convoy(PR, S2, F2, PL3))).

cut(supportmove(PR, PL, move(D, S, PL2))) :-
    move(D, S, PL2),
    supportmove(PR, PL, move(D, S, PL2)),
    province(S2),
    player(PL3),
    move(PR, S2, PL3).
cut(supportmove(PR, PL, move(D, S, PL2))) :-
    move(D, S, PL2),
    supportmove(PR, PL, move(D, S, PL2)),
    province(S2),
    player(PL3),
    convoy(PR, S2, F, PL3),
    not(cut(convoy(PR, S2, F, PL3))).

% All uncut supports
uncut(supporthold(PR, PL, H)) :- supporthold(PR, PL, H), not(cut(supporthold(PR, PL, H))).
uncut(supportconvoy(PR, PL, C)) :- supportconvoy(PR, PL, C), not(cut(supportconvoy(PR, PL, C))).
uncut(supportmove(PR, PL, M)) :- supportmove(PR, PL, M), not(cut(supportmove(PR, PL, M))).
uncut(convoy(D, S, F, PL)) :- convoy(D, S, F, PL), not(cut(convoy(D, S, F, PL))).

% All powers are determined by how many supports there are
power(hold(PR, PL), N) :- hold(PR, PL), count(X, uncut(supporthold(X, _, hold(PR, PL))), N).
power(convoy(D, S, F, PL), N) :- convoy(D, S, F, PL), count(X, uncut(supportconvoy(X, _, convoy(D, S, F, PL))), N).
power(move(D, S, PL), N) :- move(D, S, PL), count(X, uncut(supportmove(X, _, move(D, S, PL))), N).

% A move competes with power N if its power is at least N and it is not cut
competes(hold(PR, PL), N) :- validpower(N), hold(PR, PL), power(hold(PR, PL), M), M >= N.
competes(convoy(D, S, F, PL), N) :-
    validpower(N),
    uncut(convoy(D, S, F, PL)),
    power(convoy(D, S, F, PL), M),
    M >= N.
competes(move(D, S, PL), N) :- validpower(N), power(move(D, S, PL), M), M >= N.

% A move is stronger than power N if its power is overN and it is not cut
stronger(hold(PR, PL), N) :- validpower(N), hold(PR, PL), power(hold(PR, PL), M), M > N.
stronger(convoy(D, S, F, PL), N) :-
    validpower(N),
    uncut(convoy(D, S, F, PL)),
    power(convoy(D, S, F, PL), M),
    M > N.
stronger(move(D, S, PL), N) :- validpower(N), power(move(D, S, PL), M), M > N.

% Unsuccessful
unsuccess(move(D, S, PL)) :-
    move(D, S, PL),
    power(move(D, S, PL), N),
    hold(D, PL2),
    competes(hold(D, PL2), N).
unsuccess(move(D, S, PL)) :-
    move(D, S, PL),
    power(move(D, S, PL), N),
    convoy(D, S2, F, PL2),
    competes(convoy(D, S2, F, PL2), N).
unsuccess(move(D, S, PL)) :-
    move(D, S, PL),
    power(move(D, S, PL), N),
    move(D, S2, PL2),
    S2 \= S,
    competes(move(D, S2, PL2), N).

unsuccess(convoy(D, S, F, PL)) :-
    convoy(D, S, F, PL),
    power(convoy(D, S, F, PL), N),
    hold(D, PL2),
    competes(hold(D, PL2), N).
unsuccess(convoy(D, S, F, PL)) :-
    convoy(D, S, F, PL),
    power(convoy(D, S, F, PL), N),
    convoy(D, S2, F, PL2),
    S2 \= S,
    competes(convoy(D, S2, F, PL2), N).
unsuccess(convoy(D, S, F, PL)) :-
    convoy(D, S, F, PL),
    power(convoy(D, S, F, PL), N),
    move(D, S2, PL2),
    competes(move(D, S2, PL2), N).

unsuccess(hold(PR, PL)) :-
    hold(PR, PL),
    power(hold(PR, PL), N),
    hold(PR, PL2),
    stronger(hold(PR, PL2), N).
unsuccess(hold(PR, PL)) :-
    hold(PR, PL),
    power(hold(PR, PL), N),
    convoy(PR, S2, F, PL2),
    stronger(convoy(PR, S2, F, PL2), N).
unsuccess(hold(PR, PL)) :-
    hold(PR, PL),
    power(hold(PR, PL), N),
    move(PR, S2, PL2),
    stronger(move(PR, S2, PL2), N).
    

% Moves succeed if nothing is as strong as it and is uncut
success(move(D, S, PL)) :- 
    move(D, S, PL),
    not(unsuccess(move(D, S, PL))).
success(convoy(D, S, F, PL)) :-
    convoy(D, S, F, PL),
    not(unsuccess(convoy(D, S, F, PL))).
success(hold(PR, PL)) :-
    hold(PR, PL),
    not(unsuccess(hold(PR, PL))).

% Determine retreats
retreat(PR, PL) :- unsuccess(hold(PR, PL)), hold(PR, PL).

program :- open('answers.txt', write, Stream),
           (success(A), write(Stream, success(A)), write(Stream, '\n'), fail
           ; true
           ),
           (retreat(PR, PL), write(Stream, retreat(PR, PL)), write(Stream, '\n'), fail
           ; true
           ),
           close(Stream).

:- initialization(main).

main :-
    program,
    repeat,
        read(Term),
            writeln(term(Term)),
        Term=end_of_file -> halt, fail.
