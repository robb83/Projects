extends Node2D

@onready var example = $Node2D/GDExample
@onready var codeEdit = $Control/VBoxContainer/CodeEdit

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass


func _on_button_pressed():
	example.set_code(codeEdit.text)
