#version 430 core
out vec4 FragColor;
in vec3 pos;
in vec3 t_pos;
in vec2 tex;
in mat3 TBN;
uniform int num_lights;
uniform bool flashlight;

uniform vec3 camera_pos;
uniform vec3 camera_front;

const int MAX_LIGHTS = 16;

struct LightStruct{
  vec3 position;
  vec3 color;
  vec3 target;
  float attenuation;
  float radius; // Degrees
  bool is_active;
};

layout(std140, binding=0) uniform Lights
{
    LightStruct lights[MAX_LIGHTS];
};

vec3 calc_light(LightStruct light, vec3 position, vec3 camera);
LightStruct get_flashlight();
const float ambient_factor = 0.2f;

uniform sampler2D color_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;

void main()
{

  // Get the color from the lights
  vec3 out_light = vec3(0.0f, 0.0f, 0.0f);
  for(int i = 0; i < num_lights; ++i){
    out_light += calc_light(lights[i], pos, camera_pos);
  }

  // Add flashlight color
  out_light += calc_light(get_flashlight(), pos, camera_pos);
  //out_light = normalize(t_pos);

  // Sample texture color
  vec3 color = texture(color_texture, tex).rgb;

  // output color
  FragColor = vec4(
      clamp((out_light*color)+
        clamp(color*ambient_factor, 0.0f, 1.0f),
        0.0f, 1.0f), 1.0f);
}

// Calculate lighting for this fragment
vec3 calc_light(LightStruct light, vec3 point, vec3 camera){

  if(!light.is_active){
    return vec3(0.0f, 0.0f, 0.0f);
  }

  // Get the diffuse factor
  vec3 to_light = normalize(light.position - point);
  vec3 to_camera = normalize(camera - point);
  
  vec3 v_normal = texture(normal_texture, tex).rgb;
  v_normal = v_normal * 2.0 - 1.0;
  v_normal = normalize(TBN * v_normal);

  //if(dot(v_normal, -to_camera) > 0.0f){
    //v_normal = -v_normal;
  //}

  vec3 light_dir = normalize(light.target - light.position);
  float dist = distance(point, light.position);
  
  float spec_exp = 128.0f;
  float spec_mat = texture(specular_texture, tex).r;

  vec3 N = max(dot(to_light, v_normal), 0.0f)*v_normal;
  vec3 R = (2.0*N)-to_light;


  float diffuse_factor = abs(max(dot(to_light, v_normal), 0.0f))*light.attenuation;
  float specular_factor = pow(clamp(dot(R, to_camera), 0.0f, 1.0f), spec_exp)*
    spec_mat*light.attenuation;

  float loss = 1.0f / (dist/2.0f); 
  //loss = loss * (max(dot(-to_light, light_dir), 0.0f)*max(dot(-to_light, light_dir), 0.0f));
  //loss = loss / (degrees(acos(dot(-to_light, light_dir))) / 4.0f);

  if(dot(to_light, v_normal) <= 0.0f ||
      cos(radians(light.radius)) > max(dot(-to_light, light_dir), 0.0f)){
    diffuse_factor = 0.0f;
    specular_factor = 0.0f;
  }

  specular_factor *= loss;
  diffuse_factor *= loss;

  return light.color * 
    min((diffuse_factor + specular_factor), 1.0f); 
}

// Get flashlight details
LightStruct get_flashlight(){
  vec3 pos = camera_pos;
  vec3 front = camera_front;
  pos.y -= 0.5f;
  front.y -= 0.5f;
  return LightStruct(
    pos,
    vec3(1.0f, 1.0f, 0.8f),
    front,
    0.6f,
    30.0f,
    flashlight
  );
}
