#version 420 core
out vec4 FragColor;
in vec3 pos;
in vec3 norm;
in vec3 tangent;
in vec3 bittangent;
in vec2 tex;
in vec3 color;
uniform vec3 camera_pos;

uniform sampler2D tex_sampler;

const int MAX_LIGHTS = 16;

struct LightStruct{
  vec3 position;
  float angle;
  vec3 target;
  float attenuation;
  vec3 color;
  float _;
  int attenuation_type;
  int active;
};

layout(std140, binding=0) uniform Lights
{
    LightStruct lights[MAX_LIGHTS];
};


vec3 calc_light(LightStruct light, vec3 position, vec3 normal,
                vec3 camera);

const float ambient_factor = 0.20f;
const vec3 ambient_color = vec3(1.0f);

void main()
{

  // Get the color from the lights
  vec3 out_light = vec3(0.0f, 0.0f, 0.0f);
  for(int i = 0; i < MAX_LIGHTS; ++i){
    if(lights[i].active == 0){
      continue;
    }
    out_light += calc_light(lights[i], pos, norm, camera_pos);
  }

  //vec3 color = vec3(0.7f, 0.7f, 0.7f);

  vec3 ambient = ambient_color * ambient_factor;
  vec4 final_light = vec4(clamp(ambient + out_light, 0.0, 1.0), 1.0);
  FragColor = texture(tex_sampler, tex) * final_light;

}

// Calculate lighting for this fragment
vec3 calc_light(LightStruct light, vec3 point, vec3 normal,
    vec3 camera){

  // Get the diffuse factor
  vec3 to_light = normalize(light.position - point);
  vec3 v_normal = normalize(normal);
  vec3 to_camera = normalize(camera - point);
  if (!gl_FrontFacing)
    v_normal = -v_normal;

  vec3 light_dir = normalize(light.target - light.position);
  float dist = distance(point, light.position);
  
  float spec_exp = 128.0f;
  float spec_mat = 0.9f;

  vec3 R = reflect(-to_light, v_normal);

  float diffuse_factor = max(dot(v_normal, to_light), 0.0);

  float specular_factor = pow(clamp(dot(R, to_camera), 0.0f, 1.0f), spec_exp)*
    spec_mat;

  //float loss = 1.0f / (dist/4.0f); 
  float loss = 1.0 / (1.0 + dist * dist);
  if(light.attenuation_type == 0){
    loss = light.attenuation;
  }
  else if(light.attenuation_type == 1){
    loss = 1.0 / (1.0 + dist);
  }

  float theta = degrees(acos(dot(normalize(-to_light), normalize(light_dir))));
  float cutoff = light.angle;

  if(theta >= cutoff){
    diffuse_factor = 0.0f;
    specular_factor = 0.0f;
  }
  else{
    float factor = cutoff - theta;
    factor = 1 - exp(-factor*0.5);
    diffuse_factor *= factor;
    specular_factor *= factor;

  }

  specular_factor *= loss;
  diffuse_factor *= loss;

  return light.color * (diffuse_factor+specular_factor);

}

