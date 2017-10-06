import os
import sys

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
add_and_parse_library_option('nvidia-encoder', ['include'], ['lib/nvidia-375'])
add_and_parse_library_option('cuda')
add_and_parse_library_option('Bento4', [
        'Source/C++/Core',
        'Source/C++/Codecs',
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
add_and_parse_library_option('ffmpeg')
add_and_parse_library_option('gpac', ['include'], ['include'])

colors = {}
colors['cyan']   = '\033[96m'
colors['purple'] = '\033[95m'
colors['blue']   = '\033[94m'
colors['green']  = '\033[92m'
colors['yellow'] = '\033[93m'
colors['red']    = '\033[91m'
colors['end']    = '\033[0m'

# If the output is not a terminal, remove the colors
if not sys.stdout.isatty():
   for key, value in colors.iteritems():
      colors[key] = ''

compile_source_message = '%sCompiling              %s$SOURCE%s' % \
   (colors['blue'], colors['green'], colors['end'])

# compile_shared_source_message = '%sCompiling shared       %s$SOURCE%s' % \
compile_shared_source_message = '%sCompiling shared       %s$SOURCE%s $SHCCCOM $SHCXXCOM' % \
   (colors['blue'], colors['green'], colors['end'])

link_program_message = '%sLinking Program        %s$TARGET%s' % \
   (colors['blue'], colors['cyan'], colors['end'])

# link_library_message = '%sLinking Static Library %s$TARGET%s' % \
link_library_message = '%sLinking Static Library %s$TARGET%s: $LINKCOM' % \
   (colors['blue'], colors['cyan'], colors['end'])

# link_shared_library_message = '%sLinking Shared Library %s$TARGET%s' % \
link_shared_library_message = '%sLinking Shared Library %s$TARGET%s: $SHLINKCOM' % \
   (colors['blue'], colors['cyan'], colors['end'])

env = Environment()
if 'TERM' in os.environ:
    env['ENV']['TERM'] = os.environ['TERM']

env.Append(
    CXXCOMSTR = compile_source_message,
    CCCOMSTR = compile_source_message,
    SHCCCOMSTR = compile_shared_source_message,
    SHCXXCOMSTR = compile_shared_source_message,
    ARCOMSTR = link_library_message,
    SHLINKCOMSTR = link_shared_library_message,
    LINKCOMSTR = link_program_message,
    CPPPATH = [
        '#src',
        '/usr/inclue/freetype2',
        external_include_paths
    ],
    # DPATH = [
    #     '#src',
    # ],
    CFLAGS = [
        '-Wall',
        '-fPIC',
        '-m64',
        '-std=c++11',
    ],
    CFLAGS = [
        '-D=GPAC_CONFIG_DARWIN',
        '-D=CONFIG_DARWIN_GL',
        '-D=GPAC_64_BITS',
    ],
    CXXFLAGS = [
        '-Wall',
        '-fPIC',
        '-m64',
        '-std=c++11',
        '-D=GPAC_CONFIG_DARWIN',
        '-D=CONFIG_DARWIN_GL',
        '-D=GPAC_64_BITS',
    ],
    LINKFLAGS = [
        '-m64',
    ],
    LIBPATH = [
        '#src',
        external_lib_paths
    ],
)

conf = Configure(env)

if not conf.CheckLibWithHeader('mediaio-api', 'mediaio/api/instance/instance.h', 'c'):
    print('Did not find mediaio-api, exiting!')
    Exit(1)

Export('env')
Export('conf')

VariantDir('build/' + buildMode + '/src/plugin/plugin', 'src', duplicate = 0)
SConscript('src/plugin/SConscript', variant_dir = 'build/' + buildMode + '/src/plugin')
