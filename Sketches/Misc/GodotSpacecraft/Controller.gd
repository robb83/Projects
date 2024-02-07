extends Node2D

@onready var spaceship = $Spaceship
@onready var label_velocity = $CanvasLayer/Spaceship/VelocityLabel
@onready var label_orientation = $CanvasLayer/Spaceship/OrientationLabel

@onready var label_stabilization_velocity = $CanvasLayer/Stabilization/VelocityLabel
@onready var label_stabilization_orientation = $CanvasLayer/Stabilization/OrientationLabel
@onready var label_stabilization_enabled = $CanvasLayer/Stabilization/EnabledLabel

@onready var indicators = $Indicators

var autopilot_enabled = false
var autopilot_desired_orientation = 0
var autopilot_desired_velocity = 0
var stabilizer_enabled = false

var ship_width = 64
var ship_height = 24
var thruster_locations = []
var thruster_directions = []
var thruster_nodes = []
var thruster_usage = []

func heading_diff(current, desired):
	var x = (current + 2 * PI if current < desired else current) - desired
	x = x - 2 * PI if x > PI else x 
	return x

func ship_apply_thruster(index, scale = 1.0):
	var force = thruster_directions[index].rotated(spaceship.global_rotation) * scale
	var offset = thruster_locations[index].rotated(spaceship.global_rotation)
	spaceship.apply_impulse(force, offset)
	thruster_nodes[index].visible = true
	thruster_usage[index] = 30
	
func ship_stabilization():
	if not stabilizer_enabled:
		return
	
	if abs(spaceship.angular_velocity) > 0.01:
		for n in 8:
			var torque = thruster_directions[n].cross(thruster_locations[n])
			if spaceship.angular_velocity > 0 and torque > 0:
				ship_apply_thruster(n, 0.25)
			elif spaceship.angular_velocity < 0 and torque < 0:
				ship_apply_thruster(n, 0.25)
	
	if spaceship.linear_velocity.length() > 0.3:
		for n in 8:
			var force = thruster_directions[n].rotated(spaceship.global_rotation)
			if spaceship.linear_velocity.dot(force) < -0.5:
				ship_apply_thruster(n, 1)

func ship_autopilot():
	if not autopilot_enabled:
		return
	
	var rdiff = -heading_diff(spaceship.global_rotation, autopilot_desired_orientation)
	var rerror = rdiff - spaceship.angular_velocity
	if abs(rerror) > 0.02:
		for n in 8:
			var torque = thruster_directions[n].cross(thruster_locations[n])
			if rerror > 0 and torque < 0:
				ship_apply_thruster(n, 0.25)
			elif rerror < 0 and torque > 0:
				ship_apply_thruster(n, 0.25)
	
	var desired_vector = Vector2(1, 0).rotated(autopilot_desired_orientation) * autopilot_desired_velocity
	var verror = desired_vector - spaceship.linear_velocity
	if verror.length() > 0.5:
		for n in 8:
			var force = thruster_directions[n].rotated(spaceship.global_rotation)
			if verror.dot(force) > 0.5:
				ship_apply_thruster(n, 1)
		
func ship_display_ui():
	label_orientation.text = "Orientation: %d" % spaceship.global_rotation_degrees
	label_velocity.text = "Velocity: %d" % spaceship.linear_velocity.length()
	label_stabilization_orientation.text = "Orientation: %d" % rad_to_deg(autopilot_desired_orientation)
	label_stabilization_velocity.text = "Velocity: %d" % autopilot_desired_velocity
	label_stabilization_enabled.text = "Enabled: %s" % ("true" if autopilot_enabled else "false")

# Called when the node enters the scene tree for the first time.
func _ready():
	thruster_locations.push_back(Vector2( ship_width / 2, -ship_height / 2  ))
	thruster_directions.push_back(Vector2(0, 1))
	thruster_nodes.push_back(spaceship.get_node("ThrusterIndicator0"))
	thruster_usage.push_back(0)
	
	thruster_locations.push_back(Vector2( ship_width / 2, -ship_height / 2  ))
	thruster_directions.push_back(Vector2(-1, 0))
	thruster_nodes.push_back(spaceship.get_node("ThrusterIndicator1"))
	thruster_usage.push_back(0)
	
	thruster_locations.push_back(Vector2( ship_width / 2, ship_height / 2  ))
	thruster_directions.push_back(Vector2(-1, 0))
	thruster_nodes.push_back(spaceship.get_node("ThrusterIndicator2"))
	thruster_usage.push_back(0)
	
	thruster_locations.push_back(Vector2( ship_width / 2, ship_height / 2  ))
	thruster_directions.push_back(Vector2(0, -1))
	thruster_nodes.push_back(spaceship.get_node("ThrusterIndicator3"))
	thruster_usage.push_back(0)
	
	thruster_locations.push_back(Vector2( -ship_width / 2, ship_height / 2  ))
	thruster_directions.push_back(Vector2(0, -1))
	thruster_nodes.push_back(spaceship.get_node("ThrusterIndicator4"))
	thruster_usage.push_back(0)
	
	thruster_locations.push_back(Vector2( -ship_width / 2, ship_height / 2  ))
	thruster_directions.push_back(Vector2(1, 0))
	thruster_nodes.push_back(spaceship.get_node("ThrusterIndicator5"))
	thruster_usage.push_back(0)
	
	thruster_locations.push_back(Vector2( -ship_width / 2, -ship_height / 2  ))
	thruster_directions.push_back(Vector2(1, 0))
	thruster_nodes.push_back(spaceship.get_node("ThrusterIndicator6"))
	thruster_usage.push_back(0)
	
	thruster_locations.push_back(Vector2( -ship_width / 2, -ship_height / 2  ))
	thruster_directions.push_back(Vector2(0, 1))
	thruster_nodes.push_back(spaceship.get_node("ThrusterIndicator7"))
	thruster_usage.push_back(0)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var force = Vector2()
	var offset = Vector2()

	if Input.is_action_pressed("ui_up"):
		autopilot_desired_velocity += 1
		autopilot_desired_velocity = 0 if autopilot_desired_velocity < 0 else autopilot_desired_velocity
	if Input.is_action_pressed("ui_down"):
		autopilot_desired_velocity -= 1
		autopilot_desired_velocity = 0 if autopilot_desired_velocity < 0 else autopilot_desired_velocity
	if Input.is_action_pressed("ui_left"):
		autopilot_desired_orientation = wrap(autopilot_desired_orientation - 0.01, -PI, PI)
	if Input.is_action_pressed("ui_right"):
		autopilot_desired_orientation = wrap(autopilot_desired_orientation + 0.01, -PI, PI)
		
	if Input.is_action_just_pressed("utility_x"):
		stabilizer_enabled = not stabilizer_enabled
		autopilot_enabled = false if stabilizer_enabled else autopilot_enabled
	
	if Input.is_action_just_pressed("utility_y"):
		autopilot_enabled = not autopilot_enabled
		stabilizer_enabled = false if autopilot_enabled else stabilizer_enabled
		
	# manual control
	if Input.is_action_pressed("manual_w"):
		ship_apply_thruster(5)
		ship_apply_thruster(6)
	if Input.is_action_pressed("manual_s"):
		ship_apply_thruster(1)
		ship_apply_thruster(2)
	if Input.is_action_pressed("manual_a"):
		ship_apply_thruster(3)
		ship_apply_thruster(4)
	if Input.is_action_pressed("manual_d"):
		ship_apply_thruster(0)
		ship_apply_thruster(7)
		spaceship.apply_impulse(force, offset)
	if Input.is_action_pressed("manual_q"):
		ship_apply_thruster(3)
		ship_apply_thruster(7)
	if Input.is_action_pressed("manual_e"):
		ship_apply_thruster(0)
		ship_apply_thruster(4)
	
	# individual thrusters
	if Input.is_action_pressed("num_1"):
		ship_apply_thruster(0)
	if Input.is_action_pressed("num_2"):
		ship_apply_thruster(1)
	if Input.is_action_pressed("num_3"):
		ship_apply_thruster(2)
	if Input.is_action_pressed("num_4"):
		ship_apply_thruster(3)
	if Input.is_action_pressed("num_5"):
		ship_apply_thruster(4)
	if Input.is_action_pressed("num_6"):
		ship_apply_thruster(5)
	if Input.is_action_pressed("num_7"):
		ship_apply_thruster(6)
	if Input.is_action_pressed("num_8"):
		ship_apply_thruster(7)
		
	if Input.is_action_pressed("ui_cancel"):
		spaceship.linear_velocity = Vector2()
		spaceship.angular_velocity = 0
		stabilizer_enabled = false
		autopilot_enabled = false
	
	indicators.autopilot_direction = autopilot_desired_orientation
	indicators.autopilot_velocity = autopilot_desired_velocity
	ship_stabilization()
	ship_autopilot()
	ship_display_ui()
	
	for n in 8:
		thruster_usage[n] = thruster_usage[n] - 1 if thruster_usage[n] > 0 else 0
		if thruster_usage[n] == 0:
			thruster_nodes[n].visible = false
