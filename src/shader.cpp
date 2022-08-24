#include <shader.hpp>

// set shader activve
Shader &Shader::use(){
    glUseProgram(this->ID);
    return *this;
}

void Shader::compile(const char* vertexSource, const char* fragmentSource){
    // IDs
    unsigned int vertexShader, fragmentShader;

    // creating vertex shader object 
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "vertex");

    // creating frag shader object
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "fragment");

    // creating the shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "program");

    // clear shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::checkCompileErrors(unsigned int obj, std::string type){
    int success; // success boolean
    char infoLog[1024]; // info about the error  

    // if not a shader program
    if (type != "program"){
        // see how the compilation went
        glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
        // and if not successful
        if (!success){
            // find out why it is not successful
            glGetShaderInfoLog(obj, 1024, NULL, infoLog);
            std::cerr << "Error: Shader Compilation\nError Type: " << type << "\nGLAD error: " << infoLog << "\n" << std::endl;
        }
    } 
    // if it is a shader program
    else{
        // see how the linking of shaders went
        glGetProgramiv(obj, GL_LINK_STATUS, &success);
        // and if not successful
        if (!success){
            // find out why
            glGetProgramInfoLog(obj, 1024, NULL, infoLog);
            std::cerr << "Error: Program Linkage\nError Type: " << type << "\nGLAD error: " << infoLog << "\n" << std::endl; 
        }
    }
}

// from here downwards all functions...
// ...check if shader is active
// ...set a value to a uniform in one of the shaders

void Shader::setFloat(const char *name, float value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::setInteger(const char *name, int value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::setVector2f(const char *name, float x, float y, bool useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::setVector2f(const char *name, const glm::vec2 &value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::setVector3f(const char *name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::setVector3f(const char *name, const glm::vec3 &value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::setVector4f(const char *name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::setVector4f(const char *name, const glm::vec4 &value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::setMatrix4(const char *name, const glm::mat4 &matrix, bool useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}
