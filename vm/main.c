/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djoye <djoye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 10:05:40 by sdoughnu          #+#    #+#             */
/*   Updated: 2020/02/20 15:48:55 by djoye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"

void			flag_l(int *i, int c, char **v, t_vm *vm)
{
	if (*i < c && !ft_strcmp(v[*i], "-l"))
	{
		vm->l = 1;
		(*i)++;
	}
	else if (*i < c && !ft_strcmp(v[*i], "-a"))
	{
		vm->a = 1;
		(*i)++;
	}
}

int				flag(int *i, int c, char **v, t_vm *vm)
{
	if (*i + 1 < c && !ft_strcmp(v[*i], "-dump"))
	{
		vm->dump_count = ft_atoi(v[++(*i)]);
		vm->dump = 1;
		if (vm->dump_count < 0)
			return (print_fd("Invalid option in flag [-dump]\n", 2));
		(*i)++;
		return (1);
	}
	else if (*i < c && !ft_strcmp(v[*i], "-v") && (vm->color = 1) && (*i)++)
		return (1);
	else if (*i + 1 < c && !ft_strcmp(v[*i], "-d"))
	{
		vm->d_count = ft_atoi(v[++(*i)]);
		vm->d = 1;
		if (vm->d_count < 0)
			return (print_fd("Invalid option in flag [-d]\n", 2));
		(*i)++;
		return (1);
	}
	if ((!ft_strcmp(v[*i], "-dump") || !ft_strcmp(v[*i], "-d")) && *i + 1 >= c)
		return (print_fd("The flag is missing a number\n", 2));
	return (1);
}

int				parser(t_vm *vm, int ac, char **av)
{
	int			i;
	int			n;

	i = 1;
	while (i < ac)
	{
		n = -1;
		if (!flag(&i, ac, av, vm))
			return (0);
		flag_l(&i, ac, av, vm);
		if (i + 1 < ac && !ft_strcmp(av[i], "-n"))
		{
			n = ft_atoi(av[++i]);
			if (n > MAX_PLAYERS || n < 1 || !check_n(vm, (unsigned)n))
				return (print_fd("Invalid option in flag [-n]\n", 2));
			i++;
		}
		if (i < ac && !init_champ(&i, av, n, vm))
			return (0);
	}
	vm->last_champ = vm->champ[vm->players - 1];
	return (1);
}

void			init(t_vm *vm)
{
	int			i;

	ft_bzero(vm, sizeof(t_vm));
	i = -1;
	vm->dump = -1;
	vm->color = -1;
	vm->last_live = 0;
	vm->d = -1;
	vm->a = 0;
	vm->zj = 0;
	vm->players = 0;
	vm->global = 0;
	vm->l = 0;
	vm->cycles_to_die = CYCLE_TO_DIE;
	vm->live_count = 0;
	vm->checks = 0;
	vm->last_champ = NULL;
	vm->curs = NULL;
	while (++i < MAX_PLAYERS)
		vm->champ[i] = NULL;
}

int				main(int ac, char **av)
{
	t_vm		vm;

	if (ac < 2)
	{
		ft_putstr("usage: ./corewar [champ1, champ2 ...]\n");
		ft_putstr("    [-dump <num>]   dumps memory after N cycles\n");
		ft_putstr("    [-v]            visualization\n");
		ft_putstr("    [-n <num>]      set number of the next player\n");
		exit(0);
	}
	init(&vm);
	if (!(parser(&vm, ac, av)))
		exit(0);
	if (vm.players == 0)
		exit(print_fd("You have not entered champions\n", 2));
	vm.curs_alive = vm.players;
	if (!build_arena(&vm) || !cur_init(&vm) || !war_coming(&vm))
		exit(-1);
	exit(0);
}
