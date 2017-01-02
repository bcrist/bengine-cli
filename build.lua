module { name = 'cli',
   projects = {
      lib {
         limp = {
            'include/section_ids.hpp'
         },
         src = {
            'src/*.cpp',
            'src/native/*.cpp',
            'src/native/' .. toolchain .. '/*.cpp',
            pch = 'src/pch.cpp'
         },
         preprocessor = {
            'BE_CLI_IMPL'
         }
      }
   }
}
