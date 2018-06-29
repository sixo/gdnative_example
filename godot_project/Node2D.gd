extends Node2D

onready var agdn = preload("res://lib/android_gdnative.gdns").new()

func _ready():
	var msg = agdn.test()
	print("Native code returned: " + msg)
	

