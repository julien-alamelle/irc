# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marobert <marobert@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/28 16:06:19 by marobert          #+#    #+#              #
#    Updated: 2023/10/11 17:23:27 by jalamell         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	Colors from https://misc.flogisoft.com/bash/tip_colors_and_formatting#colors1
RED			=	\e[31m
GREEN		=	\e[32m
CYAN		=	\e[34m
IGREY		=	\e[48;5;249m
END			=	\e[0m
ERASE		=	\e[2K\r


DIR_INCS	=	srcs/header
SRC_DIR		=	srcs
SRC_FILES	=	main.cpp class/Server.cpp class/ServerCommande.cpp class/User.cpp class/Channel.cpp class/Commande.cpp class/Messages.cpp
BOT_FILES	=	bot.cpp class/Commande.cpp
SRCS		=	$(addprefix $(SRC_DIR)/, $(SRC_FILES))
BOT_SRCS	=	$(addprefix $(SRC_DIR)/, $(BOT_FILES))
DIR_BUILD	=	.build/
OBJS		=	$(patsubst %.cpp, $(DIR_BUILD)%.o, $(SRCS))
BOT_OBJS	=	$(patsubst %.cpp, $(DIR_BUILD)%.o, $(BOT_SRCS))
DEPS		=	$(patsubst %.cpp, $(DIR_BUILD)%.d, $(SRCS))
BOT_DEPS	=	$(patsubst %.cpp, $(DIR_BUILD)%.d, $(BOT_SRCS))
DEPS_FLAGS	=	-MMD -MP
CC			=	c++
RM			=	rm -rf
CFLAGS		=	-std=c++98 -Wall -Wextra -Werror
NAME		=	ircserv
BONUS		=	ircbot

all:		$(NAME)

bonus:		$(BONUS)

ircserv:	$(OBJS)
			@printf "[$(CYAN)Compiling$(END)] % 25s" $@
			@$(CC) $(CFLAGS) $(DIR_INCS:%=-I %) -o $@ $(OBJS)
			@printf "$(ERASE)[$(GREEN)Done$(END)] % 30s\n" $@

ircbot:		$(BOT_OBJS)
			@printf "[$(CYAN)Compiling$(END)] % 25s" $@
			@$(CC) $(CFLAGS) $(DIR_INCS:%=-I %) -o $@ $(BOT_OBJS)
			@printf "$(ERASE)[$(GREEN)Done$(END)] % 30s\n" $@

-include $(DEPS)

$(DIR_BUILD)%.o : %.cpp
			@printf "[$(CYAN)Compiling$(END)] % 25s" $(patsubst $(shell dirname $<)/%, %, $<)
			@mkdir -p $(shell dirname $@)
			@$(CC) $(CFLAGS) $(DEPS_FLAGS) $(DIR_INCS:%=-I %) -c $< -o $@
			@printf "$(ERASE)[$(GREEN)Done$(END)] % 30s\n" $(patsubst $(shell dirname $<)/%, %, $<)

clean:
			@$(RM) $(DIR_BUILD)
			@printf "[$(RED)Deleted$(END)] % 27s\n" "$(NAME) objects"

fclean:		clean
			@$(RM) $(NAME)
			@$(RM) $(BONUS)
			@printf "[$(RED)Deleted$(END)] % 27s\n" $(NAME)
			@printf "[$(RED)Deleted$(END)] % 27s\n" $(BONUS)

re:			fclean
			@make all

.PHONY: norme
norme:
			norminette $(SRCS) $(DIR_INCS) | grep -v 'OK!' | cat

.PHONY: shrek
shrek:
		@echo "$(GREEN)⢀⡴⠑⡄⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ "
		@echo "⠸⡇⠀⠿⡀⠀⠀⠀⣀⡴⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠀⠀⠀⠑⢄⣠⠾⠁⣀⣄⡈⠙⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠀⠀⠀⢀⡀⠁⠀⠀⠈⠙⠛⠂⠈⣿⣿⣿⣿⣿⠿⡿⢿⣆⠀⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠀⠀⢀⡾⣁⣀⠀⠴⠂⠙⣗⡀⠀⢻⣿⣿⠭⢤⣴⣦⣤⣹⠀⠀⠀⢀⢴⣶⣆ "
		@echo "⠀⠀⢀⣾⣿⣿⣿⣷⣮⣽⣾⣿⣥⣴⣿⣿⡿⢂⠔⢚⡿⢿⣿⣦⣴⣾⠁⠸⣼⡿ "
		@echo "⠀⢀⡞⠁⠙⠻⠿⠟⠉⠀⠛⢹⣿⣿⣿⣿⣿⣌⢤⣼⣿⣾⣿⡟⠉⠀⠀⠀⠀⠀ "
		@echo "⠀⣾⣷⣶⠇⠀⠀⣤⣄⣀⡀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠉⠈⠉⠀⠀⢦⡈⢻⣿⣿⣿⣶⣶⣶⣶⣤⣽⡹⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠀⠀⠀⠀⠀⠀⠉⠲⣽⡻⢿⣿⣿⣿⣿⣿⣿⣷⣜⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣶⣮⣭⣽⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠀⠀⠀⠀⠀⣀⣀⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀ "
		@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⠿⠿⠿⠛⠉$(END)"

.PHONY: 42
42:
		@echo "$(IGREY)   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⡴⠶⠶⠶⠖⠲⠤⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⡾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠈⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠤⠄⣴⡚⢀⣼⠏⠀⠀⠀⠀⣀⣀⠀⢀⣀⣀⠀⣴⡿⢶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠀⠈⢀⢎⢞⣁⣿⡇⠀⠀⠀⠀⠀⣇⠈⠉⠉⠀⠉⠉⣿⡂⡄⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⢄⡀⠀⠁⣄⣢⣾⣿⣾⣿⣿⡇⠀⠀⠀⠀⢀⣼⠀⠀⢀⣀⣀⣀⣹⣷⣅⣜⣮⣳⢦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠀⠀⠀⠀⠀⠀⣠⣼⣾⣿⣴⣶⣿⣿⣿⣿⣿⣿⣿⣿⡅⣰⣞⡉⢳⣤⡷⠀⠊⢭⠶⠶⠸⣿⣿⣿⣿⣿⣿⣾⣿⣟⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠈⣹⣦⠁⠀⠀⠀⠀⠀⣤⣠⣙⣿⣿⣿⣿⣿⣿⣿⣿⣿⠤⠀⠀⠀⠀⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠀⠀⢠⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡷⣭⣤⠀⠀⠀⠀⠀⠀⢤⣤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣝⣦⡀⠀⠀⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠀⠀⣾⣟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⡇⠀⠀⠀⠀⣀⣀⣀⣀⡀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠀⣾⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⢻⢯⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣄⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⠄⣲⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⠋⠁⠀⠈⣧⠳⡄⠀⠀⠀⠀⠀⡾⢯⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡁⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⠉⠁⠀⢸⡀⠀⠀⠀⢻⠀⠙⢦⡀⠀⠀⡼⠀⢸⣿⣓⣚⡺⡟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⠀⣻⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠈⣧⠀⠀⠀⠘⣇⣀⣤⣿⠶⣾⡁⠀⣸⣿⣿⣿⡇⡇⢸⠀⠉⠻⣿⣿⣿⣿⣿⣿⣿⣽⣿⡆⠀⠀ $(END)"
		@echo "$(IGREY)   ⢀⣴⣿⣿⣿⣿⣿⣿⣿⡿⠀⠀⠀⠀⠀⠀⠠⡖⠋⠉⠀⠀⠀⠀⠘⣟⠳⠿⣝⣶⣿⢿⠃⢿⣒⣒⣰⠃⠈⡆⠀⢸⣿⣿⣿⣿⣿⣿⣿⡿⣿⠇⠀⠀ $(END)"
		@echo "$(IGREY)   ⢠⣼⣿⣿⣿⣿⣿⣿⣿⠿⠀⠀⠀⠀⢰⡀⠀⠹⡄⠀⠀⠀⠀⠀⠀⠘⣗⠒⠶⠶⠾⠽⡇⢠⡏⠈⡇⠀⠀⡇⠀⣀⣿⣿⣿⣿⣿⣿⣯⠾⣿⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⢻⣿⣿⣿⣿⣿⣿⠇⢸⠀⠀⠀⠀⠀⣧⠀⠀⢳⡀⠀⠀⠀⠀⠀⠀⢸⣉⡉⠒⠲⠶⡇⢸⡇⠀⡇⠀⠸⡃⢠⠏⣻⣿⣿⣿⣿⣿⣿⣮⠁⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⢿⣿⣿⣿⣿⣿⣿⠀⠈⣇⠀⠀⠀⠀⠈⡇⠀⠀⠳⡀⠀⠀⠀⠀⠀⠈⣧⠉⠽⠿⣺⡇⠈⢷⡤⠇⠀⢀⡿⠈⡇⠸⣿⣿⣿⣿⣿⣿⠙⠀⠀⠀⠀ $(END)"
		@echo "$(IGREY)   ⠀⢸⣿⣿⣿⣿⣿⣿⠀⠀⠘⣆⠀⠀⠀⠀⡟⠀⠀⠀⠙⠢⣄⠀⠀⠀⠀⠹⣟⣛⣓⣒⡇⠀⢸⡇⠀⣠⠟⠀⢸⠀⠀⣿⣿⣿⣿⣿⣯⠀⠀⠀⠀⠀ $(END)"

.PHONY:		all clean fclean re
