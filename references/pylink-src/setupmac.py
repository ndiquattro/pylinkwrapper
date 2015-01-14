#
# Copyright (c) 1996-2005, SR Research Ltd., All Rights Reserved
#
#
# For use by SR Research licencees only. Redistribution and use in source
# and binary forms, with or without modification, are NOT permitted.
#
#
#
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in
# the documentation and/or other materials provided with the distribution.
#
# Neither name of SR Research Ltd nor the name of contributors may be used
# to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
# IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# $Date: 2007/01/09 19:24:07 $
# 
#

from distutils.core import setup, Extension
from distutils.command.install_data import install_data
import os

#data installer with improved intelligence over distutils
#data files are copied into the project directory instead
#of willy-nilly - stolen from pygame
class smart_install_data(install_data):   
    def run(self):
        #need to change self.install_dir to the actual library dir
        install_cmd = self.get_finalized_command('install')
        self.install_dir = getattr(install_cmd, 'install_lib')
        return install_data.run(self)
        
        


DESCRIPTION = """We will provide this later"""

METADATA = {
    "name":             "pylink",
    "version":          "0.1.1",
    "license":          "SR Research",
    "url":              "http://www.eyelinkinfo.com",
    "author":           "Suganthan Subramaniam",
    "author_email":     "suganthan@eyelinkinfo.com",
    "description":      "Python wrapper for ELSDK library",
    "long_description": DESCRIPTION,
}


headers = []
extensions = [Extension("pylink.pylink",
			include_dirs = ['/Users/suganthan/Library/Frameworks/eyelink_core.framework/Versions/Current/Headers/',
					'/Users/suganthan/Library/Frameworks/eyelink_core_graphics.framework/Versions/Current/Headers/',
					'/Users/suganthan/Library/Frameworks/SDL/Versions/Current/Headers/'
					],
			extra_link_args    = ['-F/Users/suganthan/Library/Frameworks','-framework','eyelink_core',  '-framework','eyelink_core_graphics',
			'-framework', 'SDL', '-framework', 'SDL_mixer','-framework','SDL_ttf', '-framework' ,'SDL_gfx'],
                        define_macros = [('MACOSX', None),('UNIX', None),('NDEBUG', None)],
                        sources      = ["csrc/eyelinkmodule_processed.c","csrc/displaycallback.c"])
                        #sources      = ["csrc/eyelinkmodule.c"])
#sources      = ["csrc/eyelinkmodule.c"])
                        ]
data_files = []
    				
PACKAGEDATA = {
       "cmdclass":    {'install_data': smart_install_data},
       "packages":    ['pylink'],
       "package_dir": {'pylink': 'lib'},
       "headers":     headers,
       "ext_modules": extensions,
       "data_files":  [['pylink', data_files]],
}

#ext_dlls = ['eyelink_core.dll','eyelink_core_audiodll.dll','eyelink_core_graphics.dll','gd.dll','jpeg.dll','libpng1.dll','SDL.dll','SDL_draw.dll','sdl_gfx.dll','SDL_image.dll','SDL_ttf.dll','zlib.dll']
#for x in ext_dlls:
#		data_files.append(os.path.abspath("/bin/%s"%(x)))

PACKAGEDATA.update(METADATA)
apply(setup, [], PACKAGEDATA)


