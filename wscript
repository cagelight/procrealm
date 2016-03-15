#!/bin/python

from waflib import *
import os

top = '.'
out = 'build'

projname = 'procrealm'
coreprog_name = projname

g_cflags = ["-Wall", "-std=gnu11", "-pthread"]

def btype_cflags(ctx):
	return {
		"DEBUG"   : g_cflags + ["-Wextra", "-Og", "-ggdb3", "-march=core2", "-mtune=native"],
		"NATIVE"  : g_cflags + ["-Ofast", "-march=native", "-mtune=native"],
		"RELEASE" : g_cflags + ["-O3", "-march=core2", "-mtune=generic"],
	}.get(ctx.env.BUILD_TYPE, g_cflags)

def options(opt):
	opt.load("compiler_c")
	opt.add_option('--build_type', dest='build_type', type="string", default='RELEASE', action='store', help="DEBUG, NATIVE, RELEASE")

def configure(ctx):
	ctx.load("compiler_c")
	ctx.check(features='c cprogram', lib='glfw', uselib_store='GLFW')
	ctx.check(features='c cprogram', lib='GL', uselib_store='GL')
	ctx.check(features='c cprogram', lib='GLEW', uselib_store='GLEW')
	btup = ctx.options.build_type.upper()
	if btup in ["DEBUG", "NATIVE", "RELEASE"]:
		Logs.pprint("PINK", "Setting up environment for known build type: " + btup)
		ctx.env.BUILD_TYPE = btup
		ctx.env.CFLAGS = btype_cflags(ctx)
		Logs.pprint("PINK", "CFLAGS: " + ' '.join(ctx.env.CFLAGS))
	else:
		Logs.error("UNKNOWN BUILD TYPE: " + btup)

def build(bld):
	coreprog = bld (
		features = "c cprogram",
		target = coreprog_name,
		source = bld.path.ant_glob('src/*.c'),
		lib = ['m', 'pthread'],
		uselib = ['GLFW', 'GL', 'GLEW']
	)
	
def clean(ctx):
	pass
