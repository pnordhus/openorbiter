sources = '''

config.cpp
form_creatematch.cpp
form_main.cpp
form_selectkey.cpp
form_selectmap.cpp
form_settings.cpp
game.cpp
gamestats.cpp
graphicsscene.cpp
graphicsview.cpp
main.cpp
map.cpp
match.cpp
matchstats.cpp
node.cpp
openorbiter.cpp
orbiter.cpp
player.cpp
spawnpoint.cpp
vector.cpp

'''


forms = '''

form_creatematch.ui
form_main.ui
form_selectkey.ui
form_selectmap.ui
form_settings.ui

'''


buildDir = 'build'


opts = Options('Build.conf')
opts.Add(PathOption('QTDIR',                  'Directory in which Qt is installed', '/usr'))
opts.Add(PathOption('BUILD_PREFIX',           'Installation prefix', '/usr/local'))
opts.Add(BoolOption('BUILD_OPENGL',           'Set to compile with OpenGL support', 1))
opts.Add(BoolOption('BUILD_SVG',              'Set to compile with SVG support', 1))
opts.Add(BoolOption('BUILD_DEBUG',            'Set to make a debug build', 0))
opts.Add(BoolOption('BUILD_PCH',              'Set to use precompiled headers', 1))
opts.Add(BoolOption('BUILD_SHORTOUTPUT',      'Set to hide compiler command lines during build time', 1))


env = Environment(tools = [ 'default', 'gch', 'qt4' ], toolpath = [ 'scons' ], options = opts)


env.BuildDir('build/src', 'src', 0)
env.BuildDir('build/ui', 'ui', 0)


if int(env['BUILD_SHORTOUTPUT']):
	env['GCHCOMSTR'] =    "CXX  H  $TARGET"
	env['GCHSHCOMSTR'] =  "CXX SH  $TARGET"
	env['CCCOMSTR'] =     "CC      $TARGET"
	env['CXXCOMSTR'] =    "CXX     $TARGET"
	env['SHCCCOMSTR'] =   "CC  S   $TARGET"
	env['SHCXXCOMSTR'] =  "CXX S   $TARGET"
	env['LINKCOMSTR'] =   "LN      $TARGET"
	env['SHLINKCOMSTR'] = "LN  S   $TARGET"
	env['ARCOMSTR'] =     "AR      $TARGET"
	env['RANLIBCOMSTR'] = "RANLIB  $TARGET"
	env['QT4_UICCOMSTR'] =        "UIC     $TARGET"
	env['QT4_MOCFROMCXXCOMSTR'] = "MOC     $TARGET"
	env['QT4_MOCFROMHCOMSTR'] =   "MOC  H  $TARGET"
	env['QT4_RCCCOMSTR'] =        "QRC     $TARGET"

if int(env['BUILD_DEBUG']):
	env.Append(CCFLAGS = [ '-g' ])
else:
	env.Append(CCFLAGS = [ '-O2', '-DNDEBUG' ])


qtModules = [ 'QtCore', 'QtGui', 'QtXml' ]

if int(env['BUILD_OPENGL']):
	qtModules += [ 'QtOpenGL' ]
	env.Append(CCFLAGS = [ '-DBUILD_OPENGL' ])


if int(env['BUILD_SVG']):
	qtModules += [ 'QtSvg' ]
	env.Append(CCFLAGS = [ '-DBUILD_SVG' ])


env.EnableQt4Modules(qtModules)


env.Append(CCFLAGS = [ '-DDATADIR="\\"' + env['BUILD_PREFIX']  + '/share/openorbiter\\""' ])
env.Append(CPPPATH = [ '#' + buildDir + '/ui' ])


if int(env['BUILD_PCH']):
	envPch = env.Clone()
	pch = envPch.Gch(target = buildDir + '/openorbiter.gch/c++', source = 'src/pch.h')
	env.Prepend(CXXFLAGS = str('-include ' + buildDir + '/openorbiter'))
else:
	pch = 'src/pch.h'
	env.Prepend(CXXFLAGS = '-include src/pch.h')


for src in Split(forms):
	env.Uic4('build/ui/' + src)


objects = []
for src in Split(sources):
	objects += env.StaticObject(buildDir + '/src/' + src)


bin = env.Program('bin/openorbiter', objects)


for src in bin[0].sources:
	env.Depends(src, pch)
