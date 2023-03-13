NAME		=	philo

SRCS		=	main.c error.c parsing.c free.c prerequisites.c

HEADER		=	philo.h

OBJS		=	${SRCS:.c=.o}

CC			=	cc

CFLAGS		=	-g3 -pthread -Wall -Werror -Wextra

RM			=	rm -f

%.o: %.c ${HEADER}
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
