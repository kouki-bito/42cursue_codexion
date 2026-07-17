#include "codexion.h"

void	ft_lstadd_back(deque **lst, deque *new)
{
	deque	*currect;

	currect = *lst;
	if (new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	while (currect->next != NULL)
	{
		currect = currect->next;
	}
	currect->next = new;
}
void	ft_lstadd_front(deque **lst, deque *new)
{
	deque	*head;

	if (new == NULL)
		return ;
	head = *lst;
	new->next = head;
	*lst = new;
}
void	ft_delete_list(deque **lst)
{
	deque	*currect;

	currect = *lst;
	*lst = currect->next;
	free(currect);
}

deque	*ft_lstnew(t_coder *coder)
{
	deque	*new;

	new = (deque *)malloc(sizeof(deque) * 1);
	if (!new)
		return (NULL);
	new->next = NULL;
	new->coder = coder;
	return (new);
}
int	ft_lstsize(deque *lst)
{
	int		length;
	deque	*current;

	if (!lst)
		return (0);
	current = lst;
	length = 0;
	while (current != NULL)
	{
		length += 1;
		current = current->next;
	}
	return (length);
}
int	ft_find_coder(deque **head, t_coder *coder)
{
	deque *current;
	if (!head)
		return (0);
	current = *head;
	while (current)
	{
		if (current->coder->id == coder->id)
		{
			return (1);
		}
		current = current->next;
	}
	return (0);
}