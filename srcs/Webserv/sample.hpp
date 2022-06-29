/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takkatao <takkatao@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 21:37:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/06/29 17:01:06 by takkatao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SRCS_WEBSERV_SAMPLE_HPP_
#define SRCS_WEBSERV_SAMPLE_HPP_

#include <string>

class ASpell;

class ATarget {
    protected:
    std::string type;

 public:
    ATarget();
    explicit ATarget(const std::string &);
    ATarget(const ATarget &);
    virtual ~ATarget();
    ATarget &operator=(const ATarget &);

    const std::string &getType() const;
    virtual ATarget *clone() const = 0;
    void getHitBySpell(const ASpell &) const;
};

#endif  // SRCS_WEBSERV_SAMPLE_HPP_
