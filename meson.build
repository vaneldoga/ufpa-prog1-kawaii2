project('kawaii_dexu', 'cpp',
	version : '0.1',
	default_options : [
		'warning_level=3',
		'cpp_std=c++23'
	]
)

cc = meson.get_compiler('cpp')

raylib_dep = dependency('raylib', required: true)

subdir('src')

executable('pokedex-cli',
	pokedex_cli_src,
	install : false,
	include_directories: [
		'include'
	],
)

executable('pokedex',
	pokedex_src,
	install : false,
	include_directories: [
		'include'
	],
	dependencies: [
		raylib_dep,
	],
)