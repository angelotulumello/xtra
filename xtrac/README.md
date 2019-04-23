# XTRAC

XTRA - An XFSM for TRAnsport Compiler

In order to compile it for producing the jar executable of the xtrac compiler, 
launch the maven goal assembly (mvn assembly:assembly).

Then for compiling an XTRA-lang souce in JSON, just java -jar xtrac.jar -i inputfile.xtra -o outputfile.json,
where the xtrac.jar is the jar with dependencies produced by the maven assembly goal.
