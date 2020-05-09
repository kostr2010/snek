CC = g++
CFLAGS = -Wall -g

MAIN = main.cpp
VIEW = view/view.o
CONTROLLER = controller/controller.o
MODEL = model/model.o
LOG = utils/log.o

VIEW_HDR = view/view.hpp
CONTROLLER_HDR = controller/controller.hpp
MODEL_HDR = model/model.hpp
LOG_HDR = utils/log.hpp

OBJS = ${MODEL} ${VIEW} ${CONTROLLER} ${LOG}
HDRS = ${MODEL_HDR} ${VIEW_HDR} ${CONTROLLER_HDR} ${LOG_HDR}

# ${OBJS}: %.o: %.cpp
# 		${CC} ${CFLAGS} $< -o $@

main: ${OBJS} ${HDRS}
	${CC} ${CFLAGS} ${MAIN} ${OBJS} -o main.out

clean:
	rm -f main.o ${OBJS}

all: main