CC = g++
CFLAGS = -Wall -g

MAIN = main.cpp
VIEW = view/view.o
CONTROLLER = controller/controller.o
MODEL = model/model.o
ENTITY = entity/entity.o
LOG = utils/log.o
DIR = utils/direction.o
VEC2 = utils/vec2.o

VIEW_HDR = view/view.hpp
CONTROLLER_HDR = controller/controller.hpp
MODEL_HDR = model/model.hpp
ENTITY_HDR = entity/entity.hpp
LOG_HDR = utils/log.hpp
DIR_HDR = utils/direction.hpp
VEC2_HDR = utils/vec2.hpp

UTILS = utils/response.hpp utils/assertm.hpp

OBJS = ${MODEL} ${VIEW} ${CONTROLLER} ${LOG} ${ENTITY} ${DIR} ${VEC2}
HDRS = ${MODEL_HDR} ${VIEW_HDR} ${CONTROLLER_HDR} ${LOG_HDR} ${ENTITY_HDR} ${DIR_HDR} ${UTILS} ${VEC2_HDR}

main: ${OBJS} ${HDRS} main.o
	${CC} ${CFLAGS} ${MAIN} ${OBJS} -o main.out

clean:
	rm -f main.o ${OBJS}

all: main