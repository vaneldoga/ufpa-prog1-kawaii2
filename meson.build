project('kawaii_dexu', 'cpp',
	version : '0.1',
	default_options : [
		'warning_level=3',
		'cpp_std=c++23'
	]
)

cc = meson.get_compiler('cpp')

raylib_dep = dependency('raylib', required: true)
add_project_arguments('-Wno-missing-field-initializers', language : 'cpp')

raygui_project = subproject('raygui')
raygui_dependency = raygui_project.get_variable('raygui_dependency')

subdir('src')

executable('pokedex-cli',
	pokedex_cli_src,
	install : false,
	include_directories: [
		'include'
	],
)

executable('pokedex',
	pokedex_gui_src,
	install : false,
	include_directories: [
		'include'
	],
	dependencies: [
		raylib_dep,
		raygui_dependency,
		dependency('fontconfig')
	],
)