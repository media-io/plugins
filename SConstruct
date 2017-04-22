EnsureSConsVersion(2, 3, 0)

buildMode = ARGUMENTS.get('mode', 'release')
if not (buildMode in ['debug', 'release']):
    raise Exception("Can't select build mode ['debug', 'release']")

env.Append(
    CPPPATH = [
        '#src',
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
    ],
)

Export('env')

VariantDir('build/' + buildMode + '/src/plugin/plugin', 'src', duplicate = 0)
SConscript('src/plugin/SConscript', variant_dir = 'build/' + buildMode + '/src/plugin')
