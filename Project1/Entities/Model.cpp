#include "Model.h"


Model::Model(const SharedState* sharedState) : shared_state_(new SharedState(*sharedState)) {

}


Model::Model(const Model& other) : shared_state_(new SharedState(*other.shared_state_)) {

}

Model::~Model() {
	delete shared_state_;
}
SharedState * Model::shared_state() const {
	return shared_state_;
}

//void Model::InitBuffer(std::string modelName, GLuint& program_id, UniqueState& unique_state)
//{
//    vector<glm::vec3> normals = this->shared_state_->normals;
//    vector<glm::vec3> vertices = this->shared_state_->vertices;
//    vector<glm::vec2> uvs = this->shared_state_->uvs;
//
//    unique_state.vao = VAO();
//    //VBO vbo = VBO();
//
//
//
//    GLuint position_id;
//    GLuint normal_id;
//    GLuint vbo_vertices;
//    GLuint vbo_normals;
//    
//
//
//    // vbo for vertices
//    glGenBuffers(1, &vbo_vertices);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
//    glBufferData(GL_ARRAY_BUFFER,  vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    // vbo for normals
//    glGenBuffers(1, &vbo_normals);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
//    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    // Get vertex attributes
//    position_id = glGetAttribLocation(program_id, "position");
//    normal_id = glGetAttribLocation(program_id, "normal");
//
//    // Allocate memory for vao
//    glGenVertexArrays(1, &unique_state.vao.ID);
//
//    // Bind to vao
//    glBindVertexArray(unique_state.vao.ID);
//
//    // Bind vertices to vao
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);    
//    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(position_id);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    // Bind normals to vao
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
//    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(normal_id);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    // Stop bind to vao
//    glBindVertexArray(0);
//
//    // Make uniform vars
//    GLuint uniform_proj = glGetUniformLocation(program_id, "projection");
//    GLuint uniform_material_ambient = glGetUniformLocation(program_id, "mat_ambient");
//    GLuint uniform_material_diffuse = glGetUniformLocation(program_id, "mat_diffuse");
//    GLuint uniform_material_specular = glGetUniformLocation(program_id, "mat_specular");
//    GLuint uniform_material_power = glGetUniformLocation(program_id, "mat_power");
//
//    // Attach to program
//    glUseProgram(program_id);
//
//    // Fill Uniforms vars
//    glUniform3fv(uniform_material_ambient, 1, glm::value_ptr(unique_state.ambient_color));
//    glUniform3fv(uniform_material_diffuse, 1, glm::value_ptr(unique_state.diffuse_color));
//    glUniform3fv(uniform_material_specular, 1, glm::value_ptr(unique_state.specular_color));
//    glUniform1f(uniform_material_power, unique_state.power);
//}
//
