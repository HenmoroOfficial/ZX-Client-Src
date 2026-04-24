#ifndef __CPPGEN_GNET_SKILL1840
#define __CPPGEN_GNET_SKILL1840
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1840:public Skill
    {
      public:
        enum
        { SKILL_ID = 1840 };
          Skill1840 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1840Stub:public SkillStub
    {
      public:
        Skill1840Stub ():SkillStub (1840)
        {
            occupation = 153;
            name = L"½Ã½¡";
            nativename = "½Ã½¡";
            icon = "½Ã½¡.dds";
            maxlevel = 20;
            maxlearn = 6;
            type = 11;
            eventflag = 1;
            baseid = 1839;
            basesp = 1;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            action[0] = "0";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "0";
            action[14] = "0";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "";
            action[20] = "";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "0";
            rangetype = 0;
            doenchant = false;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
#ifdef _SKILL_SERVER
            skill_limit = 0;
#endif
        }
        virtual ~ Skill1840Stub ()
        {
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 2, 4, 6, 8, 10, 12 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool TakeEffect (Skill * skill) const
        {
            skill->GetPlayer ()->SetPasinchp (0.01);
            skill->GetPlayer ()->SetPasinchpgen (0.2);
            return true;
        }
#endif
    };
}
#endif
