NAME	=	philo

SRCS	=	main.c error.c parsing.c free.c prerequisites.c threads.c eat_sleep_think.c forks_utils.c time.c routine.c

HEADER	=	philo.h

OBJS	=	${SRCS:.c=.o}

CC		=	cc

#CFLAGS	=	-fsanitize=thread -pthread -Wall -Werror -Wextra #check data race
#CFLAGS	=	-g3 -pthread -Wall -Werror -Wextra # check valgrind
CFLAGS	=	-pthread -Wall -Werror -Wextra

RM		=	rm -f

%.o: %.c ${HEADER} Makefile
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
	${CC} ${CFLAGS} -o ${NAME} ${OBJS}

all:		${NAME}

clean:
	${RM} ${OBJS}

fclean:		clean
	${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
