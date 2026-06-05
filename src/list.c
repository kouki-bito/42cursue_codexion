#include "codexion.h"

void	ft_lstadd_back(t_wait_list **lst, t_wait_list *new)
{
	t_wait_list	*currect;

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
void	ft_lstadd_front(t_wait_list **lst, t_wait_list *new)
{
	t_wait_list	*head;

	if (new == NULL)
		return ;
	head = *lst;
	new->next = head;
	*lst = new;
}
t_wait_list	*ft_lstnew(void *coder)
{
	t_wait_list	*new;

	new = (t_wait_list *)malloc(sizeof(t_wait_list) * 1);
	if (!new)
		return (NULL);
	new->next = NULL;
	new->coder = coder;
	return (new);
}
int	ft_lstsize(t_wait_list *lst)
{
	int			length;
	t_wait_list	*current;

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
