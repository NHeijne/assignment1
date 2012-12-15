The program is written in C++.

= Compiling 

Remark #1 : the option-std=c++0x or -std=gnu++0x must be turned on in the compiler. 
			This by default the case in the GNU gcc compiler or in its Windows ports, MinGW and TDM-GCC. 
			(This is because of the way we initialize structs.)

Remark #2 : we make use of the Boost library collection. 
			One Boost library we use, Boost Serialization, is a non-header-only binary, which means it needs 
			to be compiled separately and then linked during compilation.

== Compiling on Linux

	1. 	First acquire the Boost libraries. These are available via the package manager in most distributions 
		(e.g. Fedora, Ubuntu). Make sure to also install the precompiled serialization library (called e.g.libboost-serialization-dev).

	2. 	Then, locate the boost serialization library by typing (without the ">"):
		> locate libboost_serialization
		and copy the location of e.g. libboost_serialization-mt.so

	3. 	Then, issue the compile command by typing the following command in the source directory, the last term should be the location 
		of e.g. libboost_serialization-mt.so that you copied in the previous step:
		> c++ -o project1 Main.cpp Grammar.cpp CYKParser.cpp TreeManager.cpp /usr/lib64/libboost_serialization-mt.so


== Compiling on Windows

	This is more complex, and discouraged. We will provide some links, since a full explanation is too long.

	A tutorial focused on building Boosts non-header-only libraries (see step 5) and Eclipse integration for MinGW and Boost can be found here:
	http://theseekersquill.wordpress.com/2010/08/24/howto-boost-mingw/
	 
	A tutorial focused on Netbeans integration using Cygwin and Boost can be found here:
	http://fischerlaender.de/development/using-boost-c-libraries-with-gcc-g-under-windows

	(Note: we could send a Windows binary alongside with the code if that is appreciated.)

= Running

The program can be run with three optional arguments, namely, the treebank file filename, the test sentences file filename,
and the test sentences trees file filename, respectively.
If these are not provided, however, the program will ask for them.

= Code structure

== Class structure

=== Main.cpp:
		This provides the initialisation, of Parser.cpp mainly. Can read command line arguments.
=== Grammar.cpp:
		This represents the CFG. 
		It reads the CFG from the treebank and writes it to a XML file, or it reads the CFG from a XML file.
		It holds 2 tables (multimaps) that map the RHS's to the LHS's and vice versa.
		It holds a table (multidimensional) for the smoothing.
		It can write the left-to-right-rule table and the smoothing table to files, and load them from these files.
		Note that as such, two files are used for this serialization process.		
=== SentenceParser.cpp
		This implements the Viterbi-on-CYK algorithm.
		It can produce the derivations for a given sentence.
		Then it can print the CYK table. 
		It can write all the rules TOP-->XP for a sentence to a file (sub-assignment 2), and also
		produce a tree for the Viterbi parse in the CYK Table (sub-assignment 3).
=== TreeManager.cpp
		This is a static class.
		It is used to debinarize the trees, and reparse the %%%%%-unary-rules from the trees.
		It can return the Penn WSJ string for a tree.
=== Parser.cpp
		This class is the main controller.
		It initializes the Grammar, and then reads the test sentences file and parses it by invoking SentenceParser.
		It produces a new test-trees file, that only contains the test-trees of the corresponding test-sentences actually
		parsed (sentences with more than 16 terms are not parsed). 

== General code remarks

	First of all, the multimap "l2rTable" (a LHS-to-RHS table) in Grammar.cpp is largely useless because we only look up the LHS's of rules.
	It said both directions (LHS to RHS, and RHS to LHS) are necessary in part 1 of the assignment.
	We should delete it and rearrange the code.
	
	A C++ (STL) multimap is used in Grammar.cpp for containing the rule table.
	It maps keys to values, in which keys having multiple values is possible. Therefore we used it for the CFG representation.
	Lookup is by key. It is efficient for lookup by range, which is what we do when getting all keys being equal to e.g. "S".
	
	A C++ (Boost) unordered_map is used in CYKParser.cpp. It is the structure of the cells of the CYK table.
	No duplicate keys are allowed, i.e. a key cannot have multiple values.
	It is efficient for lookup by one key.
	We use it for the table in the CYK algorithm so that we can efficiently get and edit, and easily insert, the LHS of a rule in the cell. 
	
	For generating trees from the CYK table we use the class tree.hh. Credits go to Kasper Peeters. Information is at http://tree.phi-sci.com/documentation.html.
	
		


