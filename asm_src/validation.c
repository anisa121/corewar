/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stross <stross@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 17:54:11 by stross            #+#    #+#             */
/*   Updated: 2020/02/25 15:05:30 by stross           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static int	check_comm_bytes(char *str, int line, int row, t_head *head)
{
	int		mod;
	int		arr[2];

	if ((mod = find_command(str, 'z')) == 0)
		insruction_token(str, line, row);
	norm_check_comm_bytes(arr, arr + 1, line, row);
	if (!head->comm_ex || !head->name_ex)
		token_syntax_error(ft_strcdup(str, ' '), line, row);
	if (mod == 1 || mod == 9 || mod == 12 || mod == 15)
		c_live_zjmp_fork_lfork(str, head, mod, arr);
	if (mod == 16 || mod == 14)
		c_ld_lld(str, head, mod, arr);
	if (mod == 6 || mod == 7 || mod == 8)
		c_and_xor_or(str, head, mod, arr);
	if (mod == 4 || mod == 5)
		c_add_sub(str, head, mod, arr);
	if (mod == 10 || mod == 13)
		c_ldi_lldi(str, head, mod, arr);
	if (mod == 11)
		c_st(str, head, mod, arr);
	if (mod == 3)
		c_sti(str, head, mod, arr);
	if (mod == 2)
		c_aff(str, head, mod, arr);
	return (0);
}

static void	check_operator(char *str, t_head *head, int line, int row)
{
	t_command	*mark;
	char		*temp;

	if ((temp = label(str, line, row)))
	{
		mark = (t_command*)ft_memalloc(sizeof(t_command));
		mark->label = temp;
		mark->is_label = true;
		if (!head->comm_ex || !head->name_ex)
			token_label(temp, line, row);
		ft_list_push_back(&head->list, mark);
	}
	if (temp)
	{
		row += ft_strchlen(str, LABEL_CHAR);
		str = ft_strchr(str, LABEL_CHAR) + 1;
		while (ft_isspace(*str))
		{
			str++;
			row++;
		}
	}
	if (*str)
		check_comm_bytes(str, line, row, head);
}

static void	check_string(char *str, t_head *head, int line, int row)
{
	while (ft_isspace(*str))
	{
		row++;
		str++;
	}
	if (*str == '.')
	{
		if (ft_strnequ(str, NAME_CMD_STRING, ft_strlen(NAME_CMD_STRING)))
		{
			check_end_token(line, row, str, 1);
			norm_check_string(head, line, row, str);
		}
		else if (ft_strnequ(str, COMMENT_CMD_STRING,
				ft_strlen(COMMENT_CMD_STRING)))
		{
			check_end_token(line, row, str, 2);
			if (head->comm_ex)
				unexpected_token(line, row, str);
			else
				get_comm_bytes(str, head, line, row);
		}
		else
			unexpected_token(line, row, str);
	}
		else check_operator(str, head, line, row);
}

static void	set_struct(char **arr, t_head *head)
{
	int		i;
	char	*str;
	int		line;
	int		row;

	norm_set_struct(&line, &i);
	while (arr[i])
	{
		row = 1;
		str = arr[i];
		while (ft_isspace(*str))
		{
			str++;
			row++;
		}
		if (!*str)
		{
			line++;
			i++;
			continue ;
		}
		check_string(str, head, line, row);
		i++;
		line++;
	}
}

void		validator(char **argv, t_head *head)
{
	char		*str;
	char		**arr;

	str = get_string(argv);
	arr = split(str, '\n');
	check_lb(arr);
	remove_comments(arr);
	set_struct(arr, head);
	if (!head->comm_ex || !head->name_ex)
	{
		write(2, "Not enough data\n", 16);
		exit(1);
	}
	validate_overflow(head->list);
	validate_labels(head->list);
	head->main_str = str;
	head->split_str = arr;
}
