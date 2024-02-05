Compiled Ruleset @602 
Compiled Rule 
	--atommatch:
	--memmatch:
		spec           [1, 5]
		commit         ["_init", 0]
		loadruleset    [0, @601]
		newatom      [1, 0, 'a'_1]
		newatom      [2, 0, 0_1]
		newatom      [3, 0, 'a'_1]
		newatom      [4, 0, 0_1]
		newlink        [1, 0, 2, 0, 0]
		newlink        [3, 0, 4, 0, 0]
		proceed        []


Compiled Ruleset @601  
Compiled Rule 
	--atommatch:
	--memmatch:
		spec           [1, 5]
		branch[[
		findatom     [1, 0, 'a'_1]
		commit         ["a", 0]
		removeatom     [1, 0]
		newatom      [2, 0, 'b'_1]
		relink         [2, 0, 1, 0, 0]
		freeatom       [1]
		proceed        []
		]]
		findatom     [3, 0, 'b'_1]
		commit         ["b", 0]
		removeatom     [3, 0]
		newatom      [4, 0, 'c'_1]
		relink         [4, 0, 3, 0, 0]
		freeatom       [3]
		proceed        []


