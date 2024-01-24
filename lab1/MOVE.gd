extends MeshInstance3D


# Called when the node enters the scene tree for the first time.
func _ready():
	loading("res://dots.txt")

var t = 0
@export var arr: Array[Vector3] = []
var segment = 0
var stop = false
var path: Curve3D
var initial = Vector3(0, 0, 1)
var curve = ImmediateMesh.new()

func get_pos(segment, t):
	var pos = 1/6. * ((-t**3 + 3* t**2 -3*t +1)*arr[segment-1] + (3*t**3 - 6* t**2+4)*arr[segment] + (-3*t**3+3*t**2+3*t+1)*arr[segment+1] + t**3 * arr[segment+2])
	return pos

func get_tangent(segment, t):
	return 1/2. * ((-t**2 + 2*t -1)*arr[segment-1] + (3*t**2 -4*t)*arr[segment] + (-3*t**2+2*t+1)*arr[segment+1] + t**2 * arr[segment+2])

func get_rot_DCM(segment, t):
	var w = get_tangent(segment, t).normalized()
	var dw = (-t+1)*arr[segment-1] + (3*t-2)*arr[segment] + (-3*t+1)*arr[segment+1] + t* arr[segment+2]
	if dw == Vector3(0,0,0):
		dw = Vector3(1, 1, 1)
	var u = w.cross(dw).normalized()
	var v = w.cross(u).normalized()
	var xyz = get_pos(segment, t)
	basis.x = v
	basis.y = -w
	basis.z = u
	
func rot_obj():
	var ori = get_tangent(segment+1, t)
	var axis = initial.cross(ori)
	var angle = initial.angle_to(ori)
	rotate(axis.normalized(), angle)
	initial = ori

func draw_tang(segment, t):
	curve.clear_surfaces()
	var tang = get_tangent(segment, t)
	var pos = get_pos(segment, t)
	var meshInstance = MeshInstance3D.new();
	var material = StandardMaterial3D.new()
	material.ShadingMode = StandardMaterial3D.SHADING_MODE_UNSHADED
	material.albedo_color = Color.CHOCOLATE
	curve.surface_begin(Mesh.PRIMITIVE_LINE_STRIP, material)
	curve.surface_add_vertex(pos)
	curve.surface_add_vertex(pos+tang)
	curve.surface_end()
	meshInstance.mesh = curve
	(Engine.get_main_loop() as SceneTree).root.add_child.call_deferred(meshInstance);

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if len(arr) < 4:
		stop = true
	t += delta
	if t >= 1:
		t -= 1
		segment += 1
		if segment >= len(arr)-3:
			stop = true
	if stop:
		return
	var pos = get_pos(segment+1, t)
	position = pos
	draw_tang(segment+1, t)
	get_rot_DCM(segment+1, t)
	#rot_obj()

func loading(filename):
	file_loader(filename)
	var curve = ImmediateMesh.new()
	var meshInstance = MeshInstance3D.new();
	var material = StandardMaterial3D.new()
	material.ShadingMode = StandardMaterial3D.SHADING_MODE_UNSHADED
	material.albedo_color = Color.BLUE
	curve.surface_begin(Mesh.PRIMITIVE_LINE_STRIP, material)
	for i in range(len(arr) - 3):
		var j = 0
		while j < 1:
			var dot = get_pos(i+1, j)
			curve.surface_add_vertex(dot)
			j += 0.001
	curve.surface_end()
	meshInstance.mesh = curve
	(Engine.get_main_loop() as SceneTree).root.add_child.call_deferred(meshInstance);

func file_loader(filename):
	var file = FileAccess.open(filename, FileAccess.READ)
	var next_char = file.get_8()
	var n = 0
	var coor = 0
	var curr = Vector3(0,0,0)
	while next_char > 0:
		if next_char == 32 || next_char == 10:
			curr[coor] = n
			n = 0
			coor += 1
			if coor == 3:
				arr.append(curr)
				coor = 0
		else:
			n = n * 10 + (next_char - 48)
		next_char = file.get_8()
		
