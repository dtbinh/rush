import msvs_gen

env = { 
'root_dir' : './../../../',
'ext_libs' : {
     'ode':            'ode\\include',
     'libogg':         'libogg\\include', 
     'libvorbis':      'libvorbis\\include', 
     'libtheora':      'libtheora\\include', 
     'lua':            'lua\\src', 
     'unittest++':     'unittest++\\src',
     'tinyxml':        'tinyxml'
},
'sys_libs' : [ 'dsound.lib', 'winmm.lib', 'comctl32.lib' ],
'int_libs' : [ 
    'rb_core', 
    'rb_draw', 
    'rb_extmath', 
    'rb_extui', 
    'rb_logic', 
    'rb_legacy', 
    'rb_nature', 
    'rb_math', 
    'rb_particle', 
    'rb_physics',
    'rb_quest',
    'rb_render8',
    'rb_render9',
    'rb_scene',
    'rb_script_lua',
    'rb_sound',
    'rb_texture',
    'rb_ui',
    'rb_util',
    'rb_video'
 ],
 'compiler' : 'vc9'
}

msvs_gen.sln_gen.genSolution( env, "rush" )

print("Generated OK")