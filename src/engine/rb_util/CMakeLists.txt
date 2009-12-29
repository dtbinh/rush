include_directories(./ ${rb_include})

aux_source_directory(. rb_util_files)
add_library(rb_util STATIC ${rb_util_files})
