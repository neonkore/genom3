# -*- coding: utf-8 -*-

import os, stat

outDir = Config.getInstance().outputDir()
# make some file executable
exec_mod = stat.S_IEXEC | stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IROTH
os.chmod(outDir + "autogen", exec_mod)
os.chmod(outDir + "/autoconf/mkinstalldirs", exec_mod)
