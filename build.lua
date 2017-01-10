module 'cli' {
   lib {
      limp 'include/section_ids.hpp',
      src {
         'src/*.cpp',
         'src/native/*.cpp',
         'src/native/$(toolchain)/*.cpp',
      },
      pch_src 'src/pch.cpp',
      define 'BE_CLI_IMPL',
      link_project 'ctable'
   }
}
