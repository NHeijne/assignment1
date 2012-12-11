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

	(Note: we will try to make a makefile for final submission.)

== Compiling on Windows

	This is more complex, and discouraged. We will provide some links, since a full explanation is too long.

	A tutorial focused on building Boosts non-header-only libraries (see step 5) and Eclipse integration for MinGW and Boost can be found here:
	http://theseekersquill.wordpress.com/2010/08/24/howto-boost-mingw/
	 
	A tutorial focused on Netbeans integration using Cygwin and Boost can be found here:
	http://fischerlaender.de/development/using-boost-c-libraries-with-gcc-g-under-windows

	(Note: we could send a Windows binary alongside with the code if that is appreciated.)

= Running

The program can be run with two optional arguments, namely, the treebank file filename and the testsentences file filename, respectively.
If these are not provided, however, the program will ask for them.

= Code structure

== Class structure

=== Main.cpp:
		This provides the main control flow, which so far reads the test sentences and makes their parses being written away to a file.
=== Grammar.cpp:
		This represents the CFG. 
		It reads the CFG from the treebank and writes it to a XML file, or it reads the CFG from a XML file.
		It holds 2 tables (multimaps) that map the RHS's to the LHS's and vice versa.
=== CYKParser.cpp
		This implements the CYK algorithm.
		It can produce the derivations for a given sentence.
		Then it can print the CYK table. It also can write all the rules TOP-->XP for a sentence to a file.
=== TreeManager.cpp
		This is a class to be worked in in part 3 of the assignment.
		It is destined to e.g. debinarize the trees.

== General code remarks

	First of all, the multimap "l2rTable" (a LHS-to-RHS table) in Grammar.cpp is largely useless because we only look up the LHS's of rules.
	It says both directions (LHS to RHS, and RHS to LHS) are necessary in part 1 of the assignment.
	We now fill the "r2lTable" according to "l2rTable" but if it remains unnecessary we will directly fill the "r2lTable",
	and remove "l2rTable", since it consumes space.
	
	A C++ (STL) multimap is used in Grammar.cpp.
	It maps keys to values, in which keys having multiple values is possible. Therefore we used it for the CFG representation.
	Lookup is by key. It is efficient for lookup by range, which is what we do when getting all keys being equal to e.g. "S".
	
	A C++ (Boost) unordered_map is used in CYKParser.cpp. It is the structure of the cells of the CYK table.
	No duplicate keys are allowed, i.e. a key cannot have multiple values.
	It is efficient for lookup by one key.
	We use it for the table in the CYK algorithm so that we can efficiently get and edit, and easily insert, the LHS of a rule in the cell. 
	
	
		


