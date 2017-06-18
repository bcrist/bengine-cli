module 'cli' {
   lib {
      src {
         'src/*.cpp',
         pch_src 'src/pch.cpp'
      },
      src {
         'src/native/*.cpp',
         'src/native/$(toolchain)/*.cpp'
      },
      define 'BE_CLI_IMPL',
      link_project 'ctable'
   }
}
