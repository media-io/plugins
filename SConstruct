import os

EnsureSConsVersion(2, 3, 0)

buildMode = ARGUMENTS.get('mode', 'release')
if not (buildMode in ['debug', 'release']):
    raise Exception("Can't select build mode ['debug', 'release']")

external_include_paths=[]
external_lib_paths=[]

def add_and_parse_library_option(library_name, include=['include'], lib=['lib']):
    AddOption(
        '--' + library_name,
        dest=library_name,
        type='string',
        nargs=1,
        action='store',
        metavar='DIR',
        help='Path to root of ' + library_name + ' library.'
    )

    library_root = GetOption(library_name)
    if(library_root):
        for sub_dir in include:
            external_include_paths.append(os.path.join(library_root, sub_dir))
        for sub_dir in lib:
            external_lib_paths.append(os.path.join(library_root, sub_dir))

add_and_parse_library_option('mediaio')
add_and_parse_library_option('turingcodec')
add_and_parse_library_option('bento', [
        'Source/C++/Core'
        'Source/C++/Codecs'
        'Source/C++/MetaData'
    ], [
        'Build/Targets/x86_64-unknown-linux/Release'
    ])
add_and_parse_library_option('halide', ['include'], ['bin'])
add_and_parse_library_option('seexpr')
add_and_parse_library_option('freetype', [
        'include',
        'include/freetype2'
    ])
add_and_parse_library_option('tiff')
add_and_parse_library_option('openjpeg')
add_and_parse_library_option('asdcplib')

env = Environment()

env.Append(
    CPPPATH = [
        '#src',
        external_include_paths
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
        external_lib_paths
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
