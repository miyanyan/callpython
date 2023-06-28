def find_max(A, B, cmp):
    # 确保 A 比 B 小，便于dfs
    if len(A) > len(B):
        A, B = B, A
        reversed = True
    else:
        reversed = False
        
    ans = 0
    ans_map = {}
    lenA = len(A)
    lenB = len(B)
    
    def hash_path(path):
        res = "path:"
        for p in path:
            res += "->"
            res += "({}, {})".format(p[0], p[1])
        return res
    
    # visA = [False for i in range(lenA)]
    visB = [False for i in range(lenB)]
    path = []
    def dfs(index, cur):
        nonlocal ans
        nonlocal visB
        if (index >= lenA):
            ans = max(ans, cur)
            ans_map[hash_path(path)] = cur
            return

        # 匹配 A[index] 和 B[i]
        for i in range(lenB):
            if visB[i]:
                continue
            visB[i] = True

            if reversed:
                left = B[i]
                right = A[index]
                path.append([i, index])
            else:
                left = A[index]
                right = B[i]
                path.append([index, i])
                
            score = cmp(left, right)
            dfs(index + 1, cur + score)
            
            path.pop()
            visB[i] = False
            
    dfs(0, 0)
    return ans, ans_map
            
if __name__ == '__main__':
    A = [1, 2, 3, 4, 5, 6, 7, 8]
    B = [14, 15, 16]
    
    def cmp(left, right):
        return left + right
    
    ans, ans_map = find_max(A, B, cmp)
    print(ans, ans_map)
    