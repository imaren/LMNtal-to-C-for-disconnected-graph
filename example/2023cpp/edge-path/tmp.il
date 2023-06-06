Compiled Ruleset @602 
Compiled Rule 
	--atommatch:
	--memmatch:
		spec           [1, 1]
		jump           [L117, [0], [], []]
	--body:L117:
		spec           [1, 2]
		commit         ["_init", 0]
		loadruleset    [0, @601]
		newatom      [1, 0, 'main'_0]
		proceed        []


Compiled Ruleset @601   
Compiled Rule 
	--atommatch:
	--memmatch:
		spec           [1, 2]
		findatom     [1, 0, 'main'_0]
		jump           [L102, [0], [1], []]
	--body:L102:
		spec           [2, 6]
		commit         ["_main", 0]
		removeatom     [1, 0]
		newatom      [2, 0, 'io'.'read_line'_3]
		newatom      [3, 0, 'io'.'stdin'_1]
		newatom      [4, 0, 'loop'_3]
		newatom      [5, 0, 'io'.'stdout'_1]
		newlink        [2, 0, 3, 0, 0]
		newlink        [2, 1, 4, 1, 0]
		newlink        [2, 2, 4, 0, 0]
		newlink        [4, 2, 5, 0, 0]
		loadmodule     [0, "io"]
		loadmodule     [0, "io"]
		loadmodule     [0, "io"]
		freeatom       [1]
		proceed        []

Compiled Rule 
	--atommatch:
	--memmatch:
		spec           [1, 3]
		findatom     [1, 0, 'loop'_3]
		deref        [2, 1, 0, 0]
		func           [2, 'eof'_1]
		jump           [L107, [0], [1, 2], []]
	--body:L107:
		spec           [3, 5]
		commit         ["_loop", 0]
		removeatom     [1, 0]
		removeatom     [2, 0]
		newatom      [3, 0, 'io'.'free_port'_1]
		newatom      [4, 0, 'io'.'free_port'_1]
		relink         [3, 0, 1, 1, 0]
		relink         [4, 0, 1, 2, 0]
		loadmodule     [0, "io"]
		loadmodule     [0, "io"]
		freeatom       [1]
		freeatom       [2]
		proceed        []

Compiled Rule 
	--atommatch:
	--memmatch:
		spec           [1, 6]
		findatom     [1, 0, 'loop'_3]
		derefatom    [3, 1, 0]
		isunary        [3]
		getfunc      [4, 3]
		loadfunc     [5, 'eof'_1]
		neqfunc        [4, 5]
		jump           [L112, [0], [1, 2, 3], []]
	--body:L112:
		spec           [4, 8]
		commit         ["_loop", 0]
		removeatom     [1, 0]
		removeatom     [3, 0]
		copyatom     [4, 0, 3]
		newatom      [5, 0, 'io'.'print_line'_3]
		newatom      [6, 0, 'io'.'read_line'_3]
		newatom      [7, 0, 'loop'_3]
		relink         [5, 0, 1, 2, 0]
		newlink        [5, 1, 4, 0, 0]
		newlink        [5, 2, 7, 2, 0]
		relink         [6, 0, 1, 1, 0]
		newlink        [6, 1, 7, 1, 0]
		newlink        [6, 2, 7, 0, 0]
		loadmodule     [0, "io"]
		loadmodule     [0, "io"]
		freeatom       [1]
		freeatom       [3]
		proceed        []


