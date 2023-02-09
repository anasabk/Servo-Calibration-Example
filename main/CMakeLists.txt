set(SOURCES main.cpp ../MyServo/MyServo.cpp)
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS . ../MyServo)