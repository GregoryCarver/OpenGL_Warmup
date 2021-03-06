//// build and compile our shader program
//// ------------------------------------
//// vertex shader
//unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//glCompileShader(vertexShader);
//// check for shader compile errors
//int success;
//char infoLog[512];
//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//if (!success)
//{
//    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//}
//// fragment shader
//unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//glCompileShader(fragmentShader);
//// check for shader compile errors
//glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//if (!success)
//{
//    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//}
//// link shaders
//unsigned int shaderProgram = glCreateProgram();
//glAttachShader(shaderProgram, vertexShader);
//glAttachShader(shaderProgram, fragmentShader);
//glLinkProgram(shaderProgram);
//// check for linking errors
//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//if (!success) {
//    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//}
//glDeleteShader(vertexShader);
//glDeleteShader(fragmentShader);



//glDeleteProgram(shaderProgram);

//const char* vertexShaderSource = "#version 330 core\n"
//                                 "layout (location = 0) in vec3 aPos;\n"
//                                 "layout (location = 1) in vec3 colorIn;\n"
//
//                                 "out vec3 colorOut;\n"
//
//                                 "void main()\n"
//                                 "{\n"
//                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//                                 "   colorOut = colorIn;\n"
//                                 "}\0";
//
//const char* fragmentShaderSource = "#version 330 core\n"
//
//                                   "in vec3 colorOut;\n"
//                                   "out vec4 FragColor;\n"
//
//                                   "void main()\n"
//                                   "{\n"
//                                   "   FragColor = vec4(colorOut, 1.0f);\n"
//                                   "}\n\0";