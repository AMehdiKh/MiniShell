/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 14:26:32 by hahadiou          #+#    #+#             */
/*   Updated: 2023/04/10 16:03:37 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int main() 
{
  char *cmd = (char *)malloc(sizeof(char) * 100);
  
  while (1) 
  {
    // Prompt the user for input
    write(1, "$ ", 2);
    // Read a command from the user
    int rd = read(0, cmd, 10);
    if (rd == 0)
    {
      // Exit the shell if the user presses Ctrl+D
      exit(0);
    }
    // Remove the trailing newline character from the command
    cmd[strcspn(cmd, "\n")] = '\0';

    // Check if the command is "exit"
    if (!strcmp(cmd, "exit")) 
    {
      // Exit the shell with a status of 0
      exit(0);
    }
  }
}
