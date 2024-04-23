project('snakegame', 'c')

fs = import('fs')

sdl2_dep = dependency('sdl2')
sdl2_ttf_dep = dependency('sdl2_ttf')

copy = fs.copyfile('assets/DroidSans.ttf')

executable('snake', 'snake.c', dependencies: [ sdl2_dep, sdl2_ttf_dep ])
