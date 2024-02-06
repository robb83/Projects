extends Node2D

@onready var spaceship = $"../Spaceship"
var current_velocity_symbol : PackedVector2Array = []
var autopilot_velocity_symbol : PackedVector2Array = []

var autopilot_direction:float
var autopilot_velocity:float

# Called when the node enters the scene tree for the first time.
func _ready():
	current_velocity_symbol = gen_polygon(4, 15)
	autopilot_velocity_symbol = gen_polygon(8, 18)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	position = spaceship.position
	queue_redraw()

func gen_polygon(count, scale):
	var result : PackedVector2Array = []
	var step = 2 * PI / count
	for n in count:
		result.push_back(Vector2(0, 1).rotated(step * n) * scale)
	return result

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
	var lv = spaceship.linear_velocity
	draw_line(Vector2(), lv, Color.GREEN)
	draw_polygon_lines(current_velocity_symbol, Color.GREEN, true, lv)
	
	# autopilot movement
	var av = Vector2(1, 0).rotated(autopilot_direction) * autopilot_velocity
	draw_line(Vector2(), av, Color.GREEN)
	draw_polygon_lines(autopilot_velocity_symbol, Color.GREEN, true, av)
