from os import path
import os.path
import sys
from shutil import copyfile
Import("env")


env.Append(CXXFLAGS=["-Wno-register"])




env.Append(
  LINKFLAGS=[
      "-mfloat-abi=hard"
  ]
)

