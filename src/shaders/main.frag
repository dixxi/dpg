in vec3 vVertex;
in vec3 vNormal;
in vec3 vNormalUntransformed;
in vec3 vColor;

const vec3 lightPos = vec3(-10.0, 10.0, 10.0);
const vec4 ambient = vec4(0.4, 0.5, 0.6, 1.0);
const vec4 diffuse = vec4(0.4, 0.4, 0.4, 1.0);
const vec4 specular = vec4(0.1, 0.1, 0.1, 1.0);

const float shininess = 1.0;

void main() {
	const float z = max(vNormalUntransformed.z, 0.2);
	const vec4 color = vec4(z, z, z, 1.0);

	const vec4 materialSpecular = color;
	const vec4 materialAmbient = color;
	const vec4 materialDiffuse = color;

	vec3 L = normalize(lightPos - vVertex);
	vec3 E = normalize(-vVertex); // we are in Eye Coordinates, so EyePos is (0,0,0)
	vec3 R = normalize(-reflect(L, vNormal));

	// calculate ambient Term:
	vec4 Iamb = materialAmbient * ambient;

	// calculate diffuse Term:
	vec4 Idiff = materialDiffuse * diffuse * max(dot(vNormal, L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);

	// calculate specular Term:
	vec4 Ispec = materialSpecular * specular * pow(max(dot(R, E), 0.0), 0.3 * shininess);
	Ispec = clamp(Ispec, 0.0, 1.0);

	// write Total Color:
	gl_FragColor = vColor * (Iamb + Idiff + Ispec);
}