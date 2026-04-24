#ifndef __CPPGEN_GNET_SKILL1183
#define __CPPGEN_GNET_SKILL1183
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1183:public Skill
    {
      public:
        enum
        { SKILL_ID = 1183 };
          Skill1183 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1183Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1183Stub ():SkillStub (1183)
        {
            occupation = 25;
            name = L"¿ª¹Æ";
            nativename = "¿ª¹Æ";
            icon = "¿ª¹Æ.dds";
            maxlevel = 5;
            maxlearn = 2;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 4;
            talent_type = 1;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 4;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            action[0] = "Ú¤ÍõºÈÁî";
            action[1] = "Ú¤ÍõºÈÁî";
            action[2] = "Ú¤ÍõºÈÁî";
            action[3] = "Ú¤ÍõºÈÁî";
            action[4] = "Ú¤ÍõºÈÁî";
            action[5] = "Ú¤ÍõºÈÁî";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "Ú¤ÍõºÈÁî";
            action[16] = "Ú¤ÍõºÈÁî";
            action[17] = "Ú¤ÍõºÈÁî";
            action[18] = "Ú¤ÍõºÈÁî";
            action[19] = "Ú¤ÍõºÈÁî";
            action[20] = "Ú¤ÍõºÈÁî";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1183Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1182 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (125 * skill->GetLevel () + 12);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[2] = { 75, 75 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (18);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, INT (125 * skill->GetLevel () + 12), 18, 12 * skill->GetLevel (), 24 - 4 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (12000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio ((0.24 - 0.04 * skill->GetLevel ()) * 5);
            skill->GetVictim ()->SetDisperseanti (1);
            return true;
        }
#endif
    };
}
#endif
