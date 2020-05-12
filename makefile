CC = g++
CFLAGS = -Wall -g -lm

MAIN = main.cpp
CONTROLLER = controller/controller.o
MODEL = model/model.o
ENTITY = entity/entity.o
LOG = utils/log.o
DIR = utils/direction.o
VEC2 = utils/vec2.o
UI_TEXT = ui/text/ui_text.o

UI_HDR = ui/ui.hpp
CONTROLLER_HDR = controller/controller.hpp
MODEL_HDR = model/model.hpp
ENTITY_HDR = entity/entity.hpp
LOG_HDR = utils/log.hpp
DIR_HDR = utils/direction.hpp
VEC2_HDR = utils/vec2.hpp
UI_TEXT_HDR = ui/text/ui_text.hpp

UTILS = utils/response.hpp utils/assertm.hpp

OBJS = ${MODEL} ${CONTROLLER} ${LOG} ${ENTITY} ${DIR} ${VEC2} ${UI_TEXT}
HDRS = ${MODEL_HDR} ${UI_HDR} ${CONTROLLER_HDR} ${LOG_HDR} ${ENTITY_HDR} ${DIR_HDR} ${UTILS} ${VEC2_HDR} ${UI_TEXT_HDR}

main: ${OBJS} ${HDRS} main.o
	${CC} ${CFLAGS} ${MAIN} ${OBJS} -o main.out

clean:
	rm -f main.o ${OBJS}

all: main