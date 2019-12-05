# Vertex Cover

This repsository contains two programs. 

### Vertex Cover
The first program performs and Conjunctive Normal Form (CNF) reduction and uses
the Minisat library to solve the vertex cover problem.

### Concurrent Vertex Cover
The second program solves the vertex cover problem using the Minisat library as 
in the first program but additionally solves the vertex cover problem using two
appriximation methods. 

The first approximation selects the vertex of highest degree (most incident edges). 
Add it to your vertex cover and throw away all edges incident on that vertex. Repeat 
till no edges remain.

The first approximation selects an edge ⟨u,v⟩, and add both u and v to your vertex 
cover. Throw away all edges attached to u and v. Repeat till no edges remain.

### Compile:
Both programs can be compiled by drilling down to the build folder and executing
"cmake ../ && make".
