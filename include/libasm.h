#ifndef __LIBASM_H__ 
#define __LIBASM_H__ 

typedef unsigned long size_t;
typedef long ssize_t;

#ifndef __nonnull
#define __nonnull(args...) __attribute__((nonnull(args)))
#endif


//
// string
//

size_t	ft_strlen(const char *str) __nonnull(1);

int		ft_strcmp(const char *s1, const char *s2) __nonnull(1, 2);

char	*ft_strcpy(const char *dest, const char *src) __nonnull(1, 2);

char	*ft_strdup(const char *src) __attribute__((malloc)) __nonnull(1);

//
// io
//

ssize_t ft_write(int fd, const char *str, size_t size);

ssize_t ft_read(int fd, const char *buf, size_t size);

#endif // __LIBASM_H__ 
