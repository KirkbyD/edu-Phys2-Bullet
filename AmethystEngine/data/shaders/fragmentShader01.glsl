#version 420

in vec4 fColour;	
in vec4 fVertWorldLocation;
in vec4 fNormal;
in vec4 fUVx2;
	
uniform vec4 diffuseColour;				// use a for transparency		
uniform vec4 specularColour;

// Used to draw debug (or unlit) objects
uniform vec4 debugColour;			
uniform bool bDoNotLight;	
uniform bool bDiscard;		// Mine

// If true, then:
// - don't light
// - texture map
// - Use colour to compare to black and change alpha 
// - Use colour to compare the black for discard
//uniform bool bIsImposter;

uniform vec4 eyeLocation;

// Texture samplers
uniform sampler2D textSamp00;
uniform sampler2D textSamp01;
//uniform sampler2D textSamp02;
//uniform sampler2D textSamp03;
//uniform sampler2D textSamp04;
//uniform sampler2D textSamp05;
//uniform sampler2D textSamp06;
//uniform sampler2D textSamp07;

uniform samplerCube skyBox1;
uniform samplerCube skyBox2;
uniform bool bIsSkyBox;

uniform bool bInstance;

//uniform sampler2D textureWhatTheWhat;
uniform sampler2D discardTexture;		// Mine

//uniform sampler2D heightMap;

uniform vec2 tex_0_1_ratio;
//uniform vec4 tex_0_3_ratio;		// x = 0, y = 1, z = 2, w = 3
//uniform vec4 tex_4_7_ratio;

// Apparently, you can now load samplers into arrays, 
// instead of using the sample2DArray sampler;
// uniform sampler2D textures[10]; 	// for instance


out vec4 pixelColour;			// RGB A   (0 to 1) 

// Fragment shader
struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 30;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms

vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, vec3 vertexWorldPos, vec4 vertexSpecular );
	 
void main() {
	// Shader Type #1  	
	if ( bDoNotLight )
	{
		pixelColour.rgb = debugColour.rgb;
		pixelColour.a = 1.0f;				// NOT transparent
		return;
	}



//	if ( bIsImposter )
//	{
//		// If true, then:
//		// - don't light
//		// - texture map
//		// - Use colour to compare to black and change alpha 
//		// - Use colour to compare the black for discard
//		vec3 texRGB = texture( textSamp00, fUVx2.st ).rgb;
//		
//		pixelColour.rgb = texRGB.rgb;
//		
//		// Note that your eye doesn't see this, 
//		// Use this equation instead: 0.21 R + 0.72 G + 0.07 B
//		float grey = (texRGB.r + texRGB.g + texRGB.b)/3.0f;
//		
//		// If it's REALLY black, then discard
//		if ( grey < 0.05 ) 	{	discard; }
//		
//		// Otherwise control alpha with "black and white" amount
//		pixelColour.a = grey;
//		if ( pixelColour.a < diffuseColour.a )
//		{
//			pixelColour.a = diffuseColour.a;
//		}
//		
//		//pixelColour.a = diffuseColour.a;
//		return;
//	}

	

	if ( bIsSkyBox )
	{
		// I sample the skybox using the normal from the surface
		vec3 tex0_RGB = texture( skyBox1, fNormal.xyz ).rgb;
		vec3 tex1_RGB = texture( skyBox2, fNormal.xyz ).rgb;

		vec3 texRGB =   ( tex_0_1_ratio.x * tex0_RGB ) 
					  + ( tex_0_1_ratio.y * tex1_RGB );

		pixelColour.rgb = texRGB.rgb;
		pixelColour.a = 1.0f;				// NOT transparent
		return;
	}



	// Shader Type #2
	vec4 materialColour = diffuseColour;
	
	vec3 tex0_RGB = texture( textSamp00, fUVx2.st ).rgb;
	vec3 tex1_RGB = texture( textSamp01, fUVx2.st ).rgb;
//	vec3 tex2_RGB = texture( textSamp02, fUVx2.st ).rgb;
//	vec3 tex3_RGB = texture( textSamp03, fUVx2.st ).rgb;
//	
//	vec3 texRGB =   ( tex_0_3_ratio.x * tex0_RGB ) 
//				  + ( tex_0_3_ratio.y * tex1_RGB );
//				  + ( tex_0_3_ratio.z * tex2_RGB )
//				  + ( tex_0_3_ratio.w * tex3_RGB );
	vec3 texRGB =   ( tex_0_1_ratio.x * tex0_RGB ) 
				  + ( tex_0_1_ratio.y * tex1_RGB );
	
	vec4 outColour;

	if ( bInstance )
	{
		outColour.rgb = texRGB;
	}
	else
	{
		outColour = calcualteLightContrib( texRGB.rgb, fNormal.xyz, fVertWorldLocation.xyz, specularColour );
	}

//	//Bunny is chome (reflective)
//	vec3 eyeVector = eyeLocation.xyz - fVertWorldLocation.xyz;
//	eyeVector = normalize(eyeVector);
//	vec3 reflectVector = reflect( eyeVector, fNormal.xyz );
//	vec3 refractVector = refract( eyeVector, fNormal.xyz, 1.4f );
//	vec3 reflectColour = texture( skyBox, reflectVector.xyz ).rgb;
//	vec3 refractColour = texture( skyBox, refractVector.xyz ).rgb;
//	vec3 finalColour = 0.0f * reflectColour + 1.0f * refractColour;
//	outColour = calcualteLightContrib( surfaceColour.rgb, fNormal.xyz, fVertWorldLocation.xyz, specularColour );



	pixelColour.rgb = outColour.rgb;
	
	// Set the "a" of diffuse to set the transparency
	pixelColour.a = diffuseColour.a; 		// "a" for alpha, same as "w"
	
	if ( bDiscard ) {
		// Discard
		vec3 texDiscard_RGB = texture( discardTexture, fUVx2.st ).rgb;
		float grey = (texDiscard_RGB.r + texDiscard_RGB.g + texDiscard_RGB.b)/3.0f;
		if ( grey < 0.05f )		// Basically "black"
		{
			discard;
		}

		// Otherwise control alpha with "black and white" amount
		pixelColour.a = grey;
		if ( pixelColour.a < diffuseColour.a )
		{
			pixelColour.a = diffuseColour.a;
		}
	}
	
//	// Projector is too dim
//	pixelColour.rgb *= 1.8f;
}	



vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
		
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)


			return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		// -1 to 1
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		// If it's negative, will clamp to 0 --- range from 0 to 1
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object�s.
		float objectSpecularPower = vertexSpecular.w; 
		
//		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
//			                   * vertexSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * theLights[index].specular.rgb;
							   
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight ); 
		
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{
			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}		
		}
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );
	}
	
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}