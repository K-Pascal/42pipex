/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:45:19 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 19:49:58 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlist;
	t_list	*first;

	if (lst == NULL)
		return (NULL);
	first = ft_lstnew(f(lst->content));
	if (first == NULL)
		return (NULL);
	newlist = first;
	lst = lst->next;
	while (lst != NULL)
	{
		newlist->next = ft_lstnew(f(lst->content));
		if (newlist->next == NULL)
		{
			ft_lstclear(&first, del);
			return (NULL);
		}
		lst = lst->next;
		newlist = newlist->next;
	}
	newlist->next = NULL;
	return (first);
}
