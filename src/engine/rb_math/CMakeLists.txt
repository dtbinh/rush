include_directories(./ ${rb_include})

aux_source_directory(. rb_math_files)
add_library(rb_math STATIC ${rb_math_files})
