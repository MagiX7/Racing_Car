#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
    

	MapCreation();

    myinit();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	geometryList.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
    display();

	/*Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();*/
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
    
    

}

Cube* ModuleSceneIntro::CreateCube(vec3 pos, vec3 size, Color rgb, float mass, bool isSensor)
{
	Cube* cube = new Cube();
	cube->SetPos(pos.x,pos.y,pos.z);
	cube->size = size;
	cube->color = rgb;
	
	physBodies.add(App->physics->AddBody(*cube, mass, isSensor));
    
    
	
	return cube;
}

Cube* ModuleSceneIntro::CreateRamp(vec3 pos, vec3 size, Color rgb, float angle, vec3 pivot, float mass)
{

	Cube* cube = new Cube();
	cube->SetRotation(angle, pivot);
	cube->SetPos(pos.x, pos.y, pos.z);
	cube->size = size;
	cube->color = rgb;
    physBodies.add(App->physics->AddBody(*cube, mass, false));
	
	return cube;
}

void ModuleSceneIntro::MapCreation()
{
    // Testing Sensor cube
    //geometryList.add(CreateCube(vec3(0.0f, 2.0f, 20.0f), vec3(4.0f, 4.0f, 4.0f), Black, 0, true));
    
    // Walls
	geometryList.add(CreateCube(vec3(0, 1.0f, -173.242f), vec3(360.0f,100.0f,1.0f), Blue, 0));
	geometryList.add(CreateCube(vec3(-180.0f, 1.0f, 4.478f), vec3(1.0f,100.0f,355.597f), Blue, 0));
	geometryList.add(CreateCube(vec3(0,1.0f,181.802f), vec3(360.0f,100.0f,1.0f), Blue, 0));
	geometryList.add(CreateCube(vec3(179.432f,1.0f,4.478f), vec3(1.0f,100.0f,355.597f), Blue, 0));
	
    // First double ramp
	geometryList.add(CreateCube(vec3(67.407f,5.594f,0), vec3(14.431f,12.265f,83.72f), Red, 0));
    geometryList.add(CreateRamp(vec3(54.391f,4.978f,-0.384f), vec3(1.409f,17.848f,82.1f), Red, -45, vec3(0, 0, 1), 0));
    geometryList.add(CreateRamp(vec3(80.352f,4.978f,-0.384f), vec3(1.409f,17.848f,82.1f), Red, 45, vec3(0, 0, 1), 0));

    // Second double ramp
    geometryList.add(CreateCube(vec3(-70.473f, 5.594f, 0), vec3(14.431f, 12.265f, 83.72f), Red, 0));
    geometryList.add(CreateRamp(vec3(-83.489f, 4.978f, -0.384f), vec3(1.409f, 17.848f, 82.1f), Red, -45, vec3(0, 0, 1), 0));
    geometryList.add(CreateRamp(vec3(-57.528f, 4.978f, -0.384f), vec3(1.409f, 17.848f, 82.1f), Red, 45, vec3(0, 0, 1), 0));
	
    // Starting ramp
    geometryList.add(CreateRamp(vec3(1.0f, 1.0f, 45.0f), vec3(20.0f, 30.0f, 1.0f), Red, 45, vec3(1, 0, 0),0));


    // Single Ramp with cube
    geometryList.add(CreateCube(vec3(-19.003f,5.187f,132.797f), vec3(14.431f,10.756f,13.135f), Red, 0));
    geometryList.add(CreateRamp(vec3(-33.173f,4.978f,132.721f), vec3(1.408f,17.748f,13.142f), Red, -55.444f, vec3(0, 0, 1), 0));

    // Outside ramp and build
    geometryList.add(CreateCube(vec3(0.0f,6.895f,-151.363f), vec3(279.942f,18.513f,21.03f), Red, 0));
    geometryList.add(CreateRamp(vec3(-72.099f,6.866f,-133.19f), vec3(37.045f,2.187f,23.893f), White, 45, vec3(1, 0, 0), 0));
    geometryList.add(CreateRamp(vec3(66.429f,6.866f,-133.19f), vec3(37.045f,2.187f,23.893f), White, 45, vec3(1, 0, 0), 0));
    
}

void ModuleSceneIntro::makeCheckImage(void) {

    int i, j, c;

    for (i = 0; i < checkImageWidth; i++) {

        for (j = 0; j < checkImageHeight; j++) {

            c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;

            checkImage[i][j][0] = (GLubyte)c;

            checkImage[i][j][1] = (GLubyte)c;

            checkImage[i][j][2] = (GLubyte)c;

        }
    }
}

int ModuleSceneIntro::ImageLoad(char* filename, Image* image) {

    FILE* file;

    unsigned long size; // size of the image in bytes.

    unsigned long i; // standard counter.

    unsigned short int planes; // number of planes in image (must be 1)

    unsigned short int bpp; // number of bits per pixel (must be 24)

    char temp; // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.

    if ((file = fopen(filename, "rb")) == NULL) {

        printf("File Not Found : %s\n", filename);

        return 0;

    }

    // seek through the bmp header, up to the width/height:

    fseek(file, 18, SEEK_CUR);

    // read the width

    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {

        printf("Error reading width from %s.\n", filename);

        return 0;

    }

    //printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height

    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {

        printf("Error reading height from %s.\n", filename);

        return 0;

    }

    //printf("Height of %s: %lu\n", filename, image->sizeY);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).

    size = image->sizeX * image->sizeY * 3;

    // read the planes

    if ((fread(&planes, 2, 1, file)) != 1) {

        printf("Error reading planes from %s.\n", filename);

        return 0;

    }
   
    if (planes != 1) {

        printf("Planes from %s is not 1: %u\n", filename, planes);

        return 0;

    }

    // read the bitsperpixel
    
    if ((i = fread(&bpp, 2, 1, file)) != 1) {

        printf("Error reading bpp from %s.\n", filename);

        return 0;

    }

   
    if (bpp != 24) {

        printf("Bpp from %s is not 24: %u\n", filename, bpp);

        return 0;

    }

    // seek past the rest of the bitmap header.

    fseek(file, 24, SEEK_CUR);

    // read the data.

    image->data = (char*)malloc(size);

    if (image->data == NULL) {

        printf("Error allocating memory for color-corrected image data");

        return 0;

    }

    if ((i = fread(image->data, size, 1, file)) != 1) {

        printf("Error reading image data from %s.\n", filename);

        return 0;

    }

    for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)

        temp = image->data[i];

        image->data[i] = image->data[i + 2];

        image->data[i + 2] = temp;

    }

    // we're done.

    return 1;

}


Image* ModuleSceneIntro::loadTexture() {

    Image* image1;

    // allocate space for texture

    image1 = (Image*)malloc(sizeof(Image));

    if (image1 == NULL) {

        printf("Error allocating space for image");

        exit(0);

    }

    if (!ImageLoad("arena5.bmp", image1)) {

        exit(1);

    }

    return image1;

}



void ModuleSceneIntro::myinit(void)

{

    glClearColor(0.5, 0.5, 0.5, 0.0);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

    Image* image1 = loadTexture();

    if (image1 == NULL) {

        printf("Image was not returned from loadTexture\n");

        exit(0);

    }

    makeCheckImage();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create Texture

    glGenTextures(2, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,

        GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth,

        checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);

    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_FLAT);

}

void ModuleSceneIntro::display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glDisable(GL_TEXTURE_2D);

    Plane p(0, 1, 0, 0);
    p.axis = true;
    //p.Render();

    p2List_item<Cube*>* itemCubes = geometryList.getFirst();

    p2List_item<PhysBody3D*>* itemBodies = physBodies.getFirst();

    while (itemBodies != nullptr && itemCubes != nullptr)
    {
        itemCubes->data->Render();
        itemBodies->data->GetTransform(&itemCubes->data->transform);
        itemCubes = itemCubes->next;
        itemBodies = itemBodies->next;
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(-182.0, 0.0, 182.0);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(182.0, 0.0, 182.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(182.0, 0.0, -182.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(-182.0, 0.0, -182.0);

    glDisable(GL_TEXTURE_2D);

    glEnd();

    //glutSwapBuffers();

}