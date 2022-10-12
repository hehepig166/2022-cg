#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

// consts
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const float PI = 3.14159265;


void printVec3(glm::vec3 vv) {
    std::cout <<vv.x <<" " <<vv.y <<" " <<vv.z <<std::endl;
}

struct Draw_sphere;

//==============================================
// star
// sphere, uniform circular motion
// 
//==============================================
struct Star {
    float radius;
    glm::vec3 axis;
    float theta;
    float w;
    float o_r;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    Draw_sphere *draw_sphere;

    Star *fa;
    std::vector<Star*> subs;

    int id;

    void set_draw_sphere(Draw_sphere *ds) {draw_sphere = ds;}

    void step();

    glm::mat4 get_model();

    void draw_subs();
    void draw();
};


//==============================================
// draw
//==============================================
struct Draw_sphere {
    int X_SEG, Y_SEG;   // div
    std::vector<float> Vertices;
    std::vector<int> Indices;
    unsigned int VBO, VAO;
    GLuint EBO;
    Shader shader;

    Draw_sphere(int x_seg=50, int y_seg=50);
    ~Draw_sphere();

    void Init(int x_seg, int y_seg);

    void draw_sunlight_1(glm::vec3 s_pos, float s_radius, glm::vec3 l_pos, glm::mat4 view, glm::mat4 projection);
};


//==============================================
// others
//==============================================
void processInput(GLFWwindow *window);
bool read_stars(const char *fname, std::vector<Star*> &stars);

//==============================================
// main
//==============================================
int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout <<"usage: " <<argv[0] <<" stars.txt" <<std::endl;
        std::cout <<"stars.txt: faid axis_x axis_y axis_z theta w o_r" <<std::endl;
        return 0;
    }

    // init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "hehepig", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    Draw_sphere draw_sphere;


    //glm::vec3 s_pos(.0f);
    //glm::vec3 l_pos(5.0f, 5.0f, 5.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(.0f, .0f, -10.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    
    std::vector<Star*> stars;
    Star *sun = new Star;
    sun->radius = .1f;
    sun->axis = glm::vec3(0, 0, 1);
    sun->theta = 0;
    sun->w = 0;
    sun->o_r = 0;
    sun->model = glm::mat4(1.0f);
    sun->view = view;
    sun->projection = projection;
    sun->set_draw_sphere(&draw_sphere);
    sun->fa = NULL;
    sun->id = 0;

    stars.push_back(sun);
    if (!read_stars(argv[1], stars)) {
        std::cout <<"Failed to open " <<argv[1] <<std::endl;
        return -1;
    };
    /*
    Star s1, s2;
        s1.radius = .3f;
        s1.axis = glm::vec3(1, 1, 0);
        s1.theta = 0;
        s1.w = .03;
        s1.o_r = 1;
        s1.set_draw_sphere(&draw_sphere);
        s1.model = glm::mat4(1.0f);
        s1.view = view;
        s1.projection = projection;
        s2 = s1;
        s2.axis = glm::vec3(-1, 1, 0);
        s2.radius = .1f;
        s2.theta = 0;
        s2.o_r = .5f;
        s2.w = .07;
    */
    //std::cout <<"press enter to continue" <<std::endl;
    //std::cin.getline(NULL, 0);

    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClearColor(.2f, .3f, .3f, .0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        //draw_sphere.draw_sunlight_1(s_pos, 1.0f, l_pos, view, projection);
        /*
        s2.model = s1.get_model();
        s1.draw();
        s2.draw();
        s1.step();
        s2.step();
        std::cout <<"theta: "<<s1.theta <<" " <<s2.theta <<std::endl;
        std::cout <<"w    : "<<s1.w <<" " <<s2.w <<std::endl;
        */
        //stars[1].draw_subs();
        stars[0]->draw_subs();
        for (auto p: stars) p->step();
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


}

void show_stars(Star *x, int dep) {
    for (int i=1; i<=dep; i++) std::cout <<"  ";
    std::cout <<"[" <<x->id <<"]\t " << x->radius <<" \t( " <<x->axis.x <<" " <<x->axis.y <<" " <<x->axis.z <<") \t " <<x->theta <<" \t " <<x->w <<" \t " <<x->o_r <<std::endl;
    for (auto p: x->subs) show_stars(p, dep+1);
}

bool read_stars(const char *fname, std::vector<Star*> &stars) {
    std::ifstream fin;
    fin.open(fname, std::ios::in);
    if (!fin) return false;
    int fa, id=0;
    float radius, ax, ay, az, theta, w, o_r;
    while (1) {
        fin >>fa >>radius >>ax >>ay >>az >>theta >>w >>o_r;
        if (fin.eof() || !fin) break;
        if (fa >= stars.size()) {
            std::cout <<"err on " <<fa <<" " <<radius <<" break." <<std::endl;
            break;
        }
        stars.push_back(new Star);
        Star *a = stars.back();
        Star *f = stars[fa];
        a->id = ++id;
        a->radius = radius;
        a->axis = glm::vec3(ax, ay, az);
        a->theta = theta;
        a->w = w;
        a->o_r = o_r;
        a->view = f->view;
        a->projection = f->projection;
        a->draw_sphere = f->draw_sphere;
        a->fa = f;
        f->subs.push_back(a);
    }

    std::cout <<"[id] radius ax ay az theta w o_r" <<std::endl;
    show_stars(stars[0], 0);

    return true;
}







//==============================================
// star
//==============================================
void Star::draw() {
    glm::vec3 pos = get_model() * glm::vec4(.0f, .0f, .0f, 1.0f);
    //std::cout <<id <<" " <<pos.x <<" " <<pos.y <<" " <<pos.z <<std::endl;
    draw_sphere->draw_sunlight_1(pos, radius, glm::vec3(0, 0, 0), view, projection);
}

void Star::draw_subs() {
    draw();
    for (auto p: subs) {
        p->model = get_model();
        p->draw_subs();
    }
}

void Star::step() {
    theta += w;
    if (theta > 2*PI) theta -= 2*PI;
    if (theta < -2*PI) theta += 2*PI;
}

glm::mat4 Star::get_model() {
    glm::vec3 f_pos = model * glm::vec4(0, 0, 0, 1);

    glm::vec3 orig_pos = glm::vec3(o_r, 0, 0);
    
    glm::mat4 m1 = glm::mat4(1.0f);
    glm::mat4 m2 = glm::mat4(1.0f);
    glm::mat4 m3 = glm::mat4(1.0f);
    m1 = glm::translate(m1, glm::vec3(orig_pos.x, orig_pos.y, orig_pos.z));
    m2 = glm::rotate(m2, theta, glm::vec3(axis.x, axis.y, axis.z));
    m3 = glm::translate(m3, glm::vec3(f_pos.x, f_pos.y, f_pos.z));
    return m3 * m2 * m1;
}


//==============================================
// draw
//==============================================

Draw_sphere::Draw_sphere(int x_seg, int y_seg):
    VAO(0),VBO(0),EBO(0),
    shader("resources/4.vs", "resources/4.fs")
{
    Init(x_seg, y_seg);
}

void Draw_sphere::Init(int x_seg, int y_seg) {
    X_SEG = x_seg;
    Y_SEG = y_seg;

    Vertices.clear();
    Indices.clear();

    // Vertices
    for (int y=0; y<=Y_SEG; y++) {
        for (int x=0; x<=X_SEG; x++) {
            float a = 1.0f*x/X_SEG;
            float b = 1.0f*y/Y_SEG;
            float xPos = std::cos(a*2.0f*PI)*std::sin(b*PI);
            float yPos = std::cos(b*PI);
            float zPos = std::sin(a*2.0f*PI)*std::sin(b*PI);

            Vertices.push_back(xPos);
            Vertices.push_back(yPos);
            Vertices.push_back(zPos);
        }
    }

    // Indices
    for (int i=0; i<Y_SEG; i++) {
        for (int j=0; j<X_SEG; j++) {
            Indices.push_back(i*(X_SEG+1)+j);
            Indices.push_back((i+1)*(X_SEG+1)+j);
            Indices.push_back((i+1)*(X_SEG+1)+j+1);

            Indices.push_back(i*(X_SEG+1)+j);
            Indices.push_back((i+1)*(X_SEG+1)+j+1);
            Indices.push_back(i*(X_SEG+1)+j+1);
        }
    }

    // gl
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(float), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size()*sizeof(int), &Indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Draw_sphere::~Draw_sphere() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Draw_sphere::draw_sunlight_1(glm::vec3 s_pos, float s_radius, glm::vec3 l_pos, glm::mat4 view, glm::mat4 projection) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, s_pos);
    model = glm::scale(model, glm::vec3(s_radius, s_radius, s_radius));

    shader.use();

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("lightpos", l_pos);

    glBindVertexArray(VAO);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, X_SEG*Y_SEG*6, GL_UNSIGNED_INT, 0);

    glm::vec3 center_pos = glm::vec3(model * glm::vec4(.0f, .0f, .0f, 1.0f));

    /*
    std::cout <<"r:  " <<s_radius <<std::endl;
    printVec3(s_pos);
    printVec3(center_pos);
    printVec3(l_pos);
    */
}






//===
//===
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}