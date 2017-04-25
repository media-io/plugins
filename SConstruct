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

AddOption(
    '--turingcodec',
    dest='turingcodec',
    type='string',
    nargs=1,
    action='store',
    metavar='DIR',
    help='Path to root of turingcodec library.'
)

AddOption(
    '--bento',
    dest='bento',
    type='string',
    nargs=1,
    action='store',
    metavar='DIR',
    help='Path to root of bento library.'
)

AddOption(
    '--halide',
    dest='halide',
    type='string',
    nargs=1,
    action='store',
    metavar='DIR',
    help='Path to root of halide library.'
)

AddOption(
    '--seexpr',
    dest='seexpr',
    type='string',
    nargs=1,
    action='store',
    metavar='DIR',
    help='Path to root of seexpr library.'
)

AddOption(
    '--freetype',
    dest='freetype',
    type='string',
    nargs=1,
    action='store',
    metavar='DIR',
    help='Path to root of freetype library.'
)

AddOption(
    '--tiff',
    dest='tiff',
    type='string',
    nargs=1,
    action='store',
    metavar='DIR',
    help='Path to root of tiff library.'
)

mediaio_root = GetOption('mediaio')
mediaio_include = ''
mediaio_lib = ''
if mediaio_root:
    mediaio_include = os.path.join(mediaio_root, 'include')
    mediaio_lib = os.path.join(mediaio_root, 'lib')

turingcodec_root = GetOption('turingcodec')
turingcodec_include = ''
turingcodec_lib = ''
if turingcodec_root:
    turingcodec_include = os.path.join(turingcodec_root, 'include')
    turingcodec_lib = os.path.join(turingcodec_root, 'lib')

bento_root = GetOption('bento')
bento_include = []
bento_lib = ''
if bento_root:
    bento_include.append(os.path.join(bento_root, 'Source', 'C++', 'Core'))
    bento_include.append(os.path.join(bento_root, 'Source', 'C++', 'Codecs'))
    bento_include.append(os.path.join(bento_root, 'Source', 'C++', 'MetaData'))
    bento_lib = os.path.join(bento_root, 'Build', 'Targets', 'x86_64-unknown-linux', 'Release')

halide_root = GetOption('halide')
halide_include = []
halide_lib = []
if halide_root:
    halide_include = os.path.join(halide_root, 'include')
    # halide_lib.append(os.path.join(halide_root, 'lib'))
    halide_lib.append(os.path.join(halide_root, 'bin'))

seexpr_root = GetOption('seexpr')
seexpr_include = []
seexpr_lib = []
if seexpr_root:
    seexpr_include = os.path.join(seexpr_root, 'include')
    seexpr_lib.append(os.path.join(seexpr_root, 'lib'))

freetype_root = GetOption('freetype')
freetype_include = []
freetype_lib = []
if freetype_root:
    freetype_include.append(os.path.join(freetype_root, 'include'))
    freetype_include.append(os.path.join(freetype_root, 'include', 'freetype2'))
    freetype_lib.append(os.path.join(freetype_root, 'lib'))

tiff_root = GetOption('tiff')
tiff_include = []
tiff_lib = []
if tiff_root:
    tiff_include.append(os.path.join(tiff_root, 'include'))
    tiff_lib.append(os.path.join(tiff_root, 'lib'))


env = Environment()

env.Append(
    CPPPATH = [
        '#src',
        mediaio_include,
        turingcodec_include,
        bento_include,
        halide_include,
        seexpr_include,
        freetype_include,
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
        turingcodec_lib,
        bento_lib,
        halide_lib,
        seexpr_lib,
        freetype_lib,
    ],
)

conf = Configure(env)

if not conf.CheckLibWithHeader('mediaio-api', 'mediaio/api/instance/instance.h', 'c'):
    print 'Did not find mediaio-api, exiting!'
    Exit(1)

Export('env')
Export('conf')

VariantDir('build/' + buildMode + '/src/plugin/plugin', 'src', duplicate = 0)
SConscript('src/plugin/SConscript', variant_dir = 'build/' + buildMode + '/src/plugin')
