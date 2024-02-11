@tool
extends Area3D

func _ready():
	var shape = SphereShape3D.new()
	shape.radius = 0.25
	
	var cshape = CollisionShape3D.new()
	cshape.shape = shape
	
	add_to_group("anchor", true)
	add_child(cshape)
	cshape.owner = self
