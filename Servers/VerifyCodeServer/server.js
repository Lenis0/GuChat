const grpc = require('@grpc/grpc-js')
const message_proto = require('./proto')
const const_module = require('./const')
const { v4: uuidv4 } = require('uuid')
const emailModule = require('./email')
const redis_module = require('./redis')


async function GetVerifyCode(call, callback) {
    console.log("email is ", call.request.email)
    try {
        let query_res = await redis_module.GetRedis(const_module.code_prefix + call.request.email);
        console.log("query_res is ", query_res)
        let uniqueId = query_res;
        if (query_res == null) {
            uniqueId = uuidv4();
            if (uniqueId.length > 4) {
                uniqueId = uniqueId.substring(0, 4);
            }
            let bres = await redis_module.SetRedisExpire(const_module.code_prefix + call.request.email, uniqueId, 600)
            if (!bres) {
                callback(null, {
                    email: call.request.email,
                    error: const_module.Errors.RedisErr
                });
                return;
            }
        }

        console.log("uniqueId is ", uniqueId)
        let html_str = "您的验证码是：<h2>" + uniqueId
            + "</h2><br />请您在十分钟内输入验证码，继续完成后续注册操作。<br />"
            + "请勿把验证码告诉他人。<br /><br />"
            + "——GuChat——"
        //发送邮件
        let mailOptions = {
            from: 'GuChat <lenis0@qq.com>',
            to: call.request.email,
            subject: '欢迎您注册GuChat！',
            html: html_str,
        };

        let send_res = await emailModule.SendMail(mailOptions);
        console.log("send res is ", send_res)
        if (!send_res) {
            callback(null, {
                email: call.request.email,
                error: const_module.Errors.Exception
            });
            return;
        }
        callback(null, {
            email: call.request.email,
            error: const_module.Errors.Success
        });

    } catch (error) {
        console.log("catch error is ", error)

        callback(null, {
            email: call.request.email,
            error: const_module.Errors.Exception
        });
    }

}

function main() {
    var server = new grpc.Server()
    server.addService(message_proto.VerifyCodeService.service, { GetVerifyCode: GetVerifyCode })
    server.bindAsync('127.0.0.1:50051', grpc.ServerCredentials.createInsecure(), () => {
        // server.start()
        console.log('grpc server started')
    })
}

main()