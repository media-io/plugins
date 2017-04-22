import os

EnsureSConsVersion(2, 3, 0)

buildMode = ARGUMENTS.get('mode', 'release')
if not (buildMode in ['debug', 'release']):
    raise Exception("Can't select build mode ['debug', 'release']")

AddOption(
    '--mediaio',
    dest='mediaio',
    type='string',
    nargs=1,
    action='store',
    metavar='DIR',
    help='Path to root of mediaio library.'
)

mediaio_root = GetOption('mediaio')
mediaio_include = ''
mediaio_lib = ''
if mediaio_root:
    mediaio_include = os.path.join( mediaio_root, 'include' )
    mediaio_lib = os.path.join( mediaio_root, 'lib' )

env = Environment()

env.Append(
    CPPPATH = [
        '#src',
        mediaio_include,
    ],
    DPATH = [
        '#src',
    ],
    CXXFLAGS = [
        '-Wall',
        '-fPIC',
        '-std=c++11',
    ],
    LIBPATH = [
        '#src',
        mediaio_lib,
    ],
)

conf = Configure(env)

if not conf.CheckLibWithHeader('mediaio-api', 'mediaio/api/instance/instance.h', 'c'):
    print 'Did not find mediaio-api, exiting!'
    Exit(1)

Export('env')

VariantDir('build/' + buildMode + '/src/plugin/plugin', 'src', duplicate = 0)
SConscript('src/plugin/SConscript', variant_dir = 'build/' + buildMode + '/src/plugin')
