extends Node2D

@onready var spaceship = $"../Spaceship"
var rombus : PackedVector2Array = [
	Vector2(0, -20),
	Vector2(15, 0),
	Vector2(0, 20),
	Vector2(-15, 0)
]

var autopilot_direction:float

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	position = spaceship.position
	queue_redraw()

func draw_polygon_lines(points, color, closed = true, offset = Vector2()):
	for n in range(1, points.size()):
		draw_line(points[n - 1] + offset, points[n] + offset, color)
		
	if points.size() > 2:
		draw_line(points[-1] + offset, points[0] + offset, color)
	
func _draw():
	# orientation direction
	draw_arc(Vector2(), 150, 0, 360, 100, Color.WEB_GREEN)
	draw_line(Vector2(100, 0).rotated(spaceship.global_rotation), Vector2(150, 0).rotated(spaceship.global_rotation), Color.WEB_GREEN)
	draw_line(Vector2(130, 0).rotated(autopilot_direction), Vector2(150, 0).rotated(autopilot_direction), Color.DARK_GREEN, 3)
	
	# liear movement
	var linear_velocity = spaceship.linear_velocity
	draw_line(Vector2(), linear_velocity, Color.GREEN)
	draw_polygon_lines(rombus, Color.GREEN, true, linear_velocity)
